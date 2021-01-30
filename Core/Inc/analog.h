/*
 * analog.h
 *
 *  Created on: Jan. 30, 2021
 *      Author: SG-O
 */

#ifndef INC_ANALOG_H_
#define INC_ANALOG_H_

#include "stm32g0xx_hal.h"

#define ANALOG_OVERSAMPLE 4

ADC_HandleTypeDef *ANALOG_hadc;

void ANALOG_Setup(ADC_HandleTypeDef *hadc);
void ANALOG_StartConversion(void);
void ANALOG_ForceMultiSample(uint8_t times);
uint32_t ANALOG_GetVRef(void);
uint32_t ANALOG_GetVID(void);
uint32_t ANALOG_GetmaMotor(void);

uint8_t ANALOG_GetSlot(void);

#endif /* INC_ANALOG_H_ */
