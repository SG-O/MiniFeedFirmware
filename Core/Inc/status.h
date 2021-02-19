/*
 * status.h
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
 *  Created on: Jan. 20, 2021
 *      Author: SG-O
 */

#ifndef INC_STATUS_H_
#define INC_STATUS_H_

#include "stm32g0xx_hal.h"

#define STATUS_ERROR 0
#define STATUS_READY 1
#define STATUS_BUSY 2

#define ERROR_NO_ERROR 0
#define ERROR_UNKNOWN 1
#define ERROR_MOTOR_STALL 2
#define ERROR_CONFIG_ERROR 3
#define ERROR_OUT_OF_COMP 4
#define ERROR_NO_TAPE 5
#define ERROR_HARDWARE_FAIL 6
#define ERROR_POWER 7
#define ERROR_WDT 8
#define ERROR_INITIALISATION_FAILED 9
#define ERROR_INVALID_INPUT 10
#define ERROR_UNKNOWN_METHOD 11
#define ERROR_USER_CAUSED 12
#define ERROR_CRC 13
#define ERROR_BUSY 14

uint8_t STATUS_GetStatus(void);
uint8_t ERROR_GetError(void);
void ERROR_SetError(uint8_t errorCode);
void ERROR_ResetError(void);
void STATUS_SetReady(void);
void STATUS_SetBusy(void);


#endif /* INC_STATUS_H_ */
