/*
 * analog.c
 *
 * Copyright 2021 SG-O (Joerg Bayer)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 *  Created on: Jan. 30, 2021
 *      Author: SG-O
 */

#include "analog.h"

uint32_t adc[ANALOG_CHANNELS];
uint32_t dmaBuffer[ANALOG_CHANNELS];
uint32_t vRefInt[ANALOG_OVERSAMPLE];
uint32_t vID[ANALOG_OVERSAMPLE];
uint32_t maMotor = 0;
uint32_t vRef;

uint8_t indexOversample = 0;

uint8_t busy = 0;

void ANALOG_StartConversion() {
	while (busy == 1) {
		HAL_Delay(1);
	}
	busy = 1;
	HAL_ADC_Start_DMA(ANALOG_hadc, dmaBuffer, ANALOG_CHANNELS);
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
	for (uint8_t i = 0; i < ANALOG_SLOTS; i++) {
		if ((idVoltage >= vComp) && (idVoltage < (vComp + ANALOG_VOLTAGE_STEP))) {
			return i;
		}
		vComp += ANALOG_VOLTAGE_STEP;
	}
	return ANALOG_SLOTS;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	for (uint8_t i = 0; i < ANALOG_CHANNELS; i++)
	{
	   adc[i] = dmaBuffer[i];
	}

	vRefInt[indexOversample] = adc[ANALOG_VREF_CHANNEL];
	vID[indexOversample] = adc[ANALOG_ID_CHANNEL];
	indexOversample++;

	if (indexOversample >= ANALOG_OVERSAMPLE) indexOversample = 0;
	if (indexOversample >= ANALOG_OVERSAMPLE) indexOversample = 0;

	maMotor = __HAL_ADC_CALC_DATA_TO_VOLTAGE(vRef, adc[ANALOG_MOTOR_CHANNEL], ADC_RESOLUTION_12B);
	HAL_ADC_Stop_DMA(hadc);
	busy = 0;
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc) {
	HAL_ADC_Stop_DMA(hadc);
	busy = 0;
}
