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

/* STATUS CODES */
#define STATUS_ERROR 0
#define STATUS_READY 1
#define STATUS_BUSY 2

//RESERVED 3 - 253
//UNKNOWN_(SPECIAL) 254
//RESERVED 255

/* ERROR CODES */
#define ERROR_NO_ERROR 0
#define ERROR_UNKNOWN 1
#define ERROR_HARDWARE_FAIL 2
#define ERROR_POWER 3
#define ERROR_WDT 4
#define ERROR_INITIALISATION_FAILED 5
#define ERROR_INVALID_INPUT 6
#define ERROR_UNKNOWN_METHOD 7
#define ERROR_USER_CAUSED 8
#define ERROR_CRC 9
#define ERROR_BUSY 10

//RESERVED 11 - 31

#define ERROR_MOTOR_STALL 32
#define ERROR_CONFIG_ERROR 33
#define ERROR_OUT_OF_COMP 34
#define ERROR_NO_TAPE 35
#define ERROR_STORAGE_INIT 36
#define ERROR_STORAGE_HEADER 37
#define ERROR_STORAGE_WRITE 38
#define ERROR_STORAGE_CONFIG_COUNT 39
#define ERROR_STORAGE_COUNTER_COUNT 40

//RESERVED 41 - 63
//MAINBOARD_SPECIFIC 64 - 127

#ifdef DEBUG
#define ERROR_DEBUG_0 128
#define ERROR_DEBUG_1 129
#define ERROR_DEBUG_2 130
#define ERROR_DEBUG_3 131
#define ERROR_DEBUG_4 132
#define ERROR_DEBUG_5 133
#define ERROR_DEBUG_6 134
#define ERROR_DEBUG_7 135
#define ERROR_DEBUG_8 136
#define ERROR_DEBUG_9 137
#define ERROR_DEBUG_10 138
#define ERROR_DEBUG_11 139
#define ERROR_DEBUG_12 140
#define ERROR_DEBUG_13 141
#define ERROR_DEBUG_14 142
#define ERROR_DEBUG_15 143
#endif

//RESERVED 144 - 253
//UNKNOWN_(SPECIAL) 254
//RESERVED 255


uint8_t STATUS_GetStatus(void);
uint8_t ERROR_GetError(void);
void ERROR_SetError(uint8_t errorCode);
void ERROR_ResetError(void);
void STATUS_SetReady(void);
void STATUS_SetBusy(void);


#endif /* INC_STATUS_H_ */
