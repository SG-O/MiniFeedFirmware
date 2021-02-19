/*
 * com.h
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
 *  Created on: Jan. 21, 2021
 *      Author: SG-O
 */

#ifndef INC_COM_H_
#define INC_COM_H_

#include "main.h"
#include "drive.h"
#include "persist.h"
#include "crc.h"
#include "hardware.h"
#include "opcode.h"
#include "value.h"

#define COM_BUFFER_SIZE 256

uint8_t COM_address;
I2C_HandleTypeDef *COM_hi2c;

void COM_Setup(I2C_HandleTypeDef *hi2c);
void COM_SetAddress();
uint8_t COM_GetAddress(void);
void COM_PutUnsigned8(uint8_t out);
void COM_GenerateLongCRC(void);
void COM_GenerateShortCRC(void);
void COM_PutUnsigned16(uint16_t out);
void COM_PutSigned16(int16_t out);
void COM_PutUnsigned32(uint32_t out);
void COM_PutSigned32(int32_t out);

#endif /* INC_COM_H_ */
