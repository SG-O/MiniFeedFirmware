/*
 * analog.c
 *
 *  Created on: Jan. 30, 2021
 *      Author: SG-O
 */

#include "analog.h"

uint32_t adc[4], dmaBuffer[4], vRefInt[ANALOG_OVERSAMPLE], vID[ANALOG_OVERSAMPLE];  // define variables
uint32_t maMotor = 0;
uint32_t vRef;

uint8_t indexOversample = 0;

uint8_t busy = 0;

void ANALOG_StartConversion() {
	while (busy == 1) {
		HAL_Delay(1);
	}
	busy = 1;
	HAL_ADC_Start_DMA(ANALOG_hadc, dmaBuffer, 4);
}

void ANALOG_ForceConversion() {
	ANALOG_StartConversion();
	while (busy == 1) {
		HAL_Delay(1);
	}
}

void ANALOG_ForceMultiSample(uint8_t times) {
	HAL_ADC_GetState(ANALOG_hadc);
	for (int i = 0; i < times; i++) {
		ANALOG_ForceConversion();
	}
}

uint32_t ANALOG_ResolveOversample(uint32_t *data) {
	uint32_t result = 0;
	for(uint8_t i = 0; i < ANALOG_OVERSAMPLE; i++) {
		result += data[i];
	}
	result /= ANALOG_OVERSAMPLE;
	return result;
}

void ANALOG_Calibration() {
	while (busy == 1) {
		HAL_Delay(1);
	}
	busy = 1;
	HAL_ADCEx_Calibration_Start(ANALOG_hadc);
	busy = 0;
	ANALOG_ForceMultiSample(ANALOG_OVERSAMPLE);
	vRef = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(ANALOG_ResolveOversample(vRefInt), ADC_RESOLUTION_12B);
}

void ANALOG_Setup(ADC_HandleTypeDef *hadc) {
	ANALOG_hadc = hadc;
	indexOversample = 0;
	ANALOG_Calibration();
}

uint32_t ANALOG_GetVRef() {
	return __HAL_ADC_CALC_VREFANALOG_VOLTAGE(ANALOG_ResolveOversample(vRefInt), ADC_RESOLUTION_12B);
}

uint32_t ANALOG_GetVID() {
	return __HAL_ADC_CALC_DATA_TO_VOLTAGE(vRef, ANALOG_ResolveOversample(vID), ADC_RESOLUTION_12B);
}

uint32_t ANALOG_GetmaMotor() {
	return maMotor;
}

uint8_t ANALOG_GetSlot() {
	uint32_t idVoltage = ANALOG_GetVID();
	uint32_t vComp = 23;
	for (uint8_t i = 0; i < 64; i++) {
		if ((idVoltage >= vComp) && (idVoltage < (vComp + 47))) {
			return i;
		}
		vComp += 47;
	}
	return 64;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	for (uint8_t i = 0; i < 4; i++)
	{
	   adc[i] = dmaBuffer[i];
	}

	vRefInt[indexOversample] = adc[3];
	vID[indexOversample] = adc[1];
	indexOversample++;

	if (indexOversample >= ANALOG_OVERSAMPLE) indexOversample = 0;
	if (indexOversample >= ANALOG_OVERSAMPLE) indexOversample = 0;

	maMotor = (adc[0]*3300)>>12;
	HAL_ADC_Stop_DMA(hadc);
	busy = 0;
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc) {
	HAL_ADC_Stop_DMA(hadc);
	busy = 0;
}
