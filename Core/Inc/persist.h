/*
 * persist.h
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

#ifndef INC_PERSIST_H_
#define INC_PERSIST_H_

#include "main.h"
#include "eeprom.h"

#define PERSIST_INIT_I2C_ADDRESS 0x50
#define PERSIST_MIN_I2C_ADDRESS 0x10
#define PERSIST_MAX_I2C_ADDRESS 0x50

#define PERSISt_SHORT_ID_LENGTH 6
#define PERSISt_LONG_ID_LENGTH 127

I2C_HandleTypeDef *PERSIST_hi2c;

void PERSIST_Setup(I2C_HandleTypeDef *hi2c);

void PERSIST_SetI2CAddress(uint8_t address);

void PERSIST_SetPartPitch(uint8_t partPitch);
void PERSIST_SetFeedSpeed(uint8_t feedSpeed);
void PERSIST_SetDisplayBrightness(uint8_t displayBrightness);
void PERSIST_SetMotorDir(uint8_t motorDir);
void PERSIST_SetMotorSlowDelay(uint16_t motorSlowDelay);
void PERSIST_SetTotalParts(int32_t totalParts);
void PERSIST_SetRemainingParts(int32_t remainingParts);
void PERSIST_SetLowPartWarn(uint16_t lowPartWarn);
void PERSIST_SetShortPartID(char* shortPartID, uint8_t length);
void PERSIST_SetLongPartID(char* longPartID, uint8_t length);

uint8_t PERSIST_GetI2CAddress(void);

uint8_t PERSIST_GetPartPitch(void);
uint8_t PERSIST_GetFeedSpeed(void);
uint8_t PERSIST_GetDisplayBrightness(void);
uint8_t PERSIST_GetMotorDir(void);
uint16_t PERSIST_GetMotorSlowDelay(void);
int32_t PERSIST_GetTotalParts(void);
int32_t PERSIST_GetRemainingParts(void);
uint16_t PERSIST_GetLowPartWarn(void);
char* PERSIST_GetShortPartID(void);
char* PERSIST_GetLongPartID(void);
uint64_t PERSIST_GetTotalFeeds(void);

void PERSIST_DecRemainingParts(void);

#endif /* INC_PERSIST_H_ */
