/*
 * analog.h
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

#ifndef INC_ANALOG_H_
#define INC_ANALOG_H_

#include "stm32g0xx_hal.h"

#define ANALOG_OVERSAMPLE 4
#define ANALOG_CHANNELS 4
#define ANALOG_VREF_CHANNEL 3
#define ANALOG_TEMP_CHANNEL 2
#define ANALOG_ID_CHANNEL 1
#define ANALOG_MOTOR_CHANNEL 0

#define ANALOG_VOLTAGE_STEP 47
#define ANALOG_SLOTS 64

ADC_HandleTypeDef *ANALOG_hadc;

void ANALOG_Setup(ADC_HandleTypeDef *hadc);
void ANALOG_StartConversion(void);
void ANALOG_ForceMultiSample(uint8_t times);
uint32_t ANALOG_GetVRef(void);
uint32_t ANALOG_GetVID(void);
uint32_t ANALOG_GetmaMotor(void);

uint8_t ANALOG_GetSlot(void);

#endif /* INC_ANALOG_H_ */
