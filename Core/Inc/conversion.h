/*
 * conversion.h
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
 *  Created on: Jan. 24, 2021
 *      Author: SG-O
 */

#ifndef INC_CONVERSION_H_
#define INC_CONVERSION_H_

#include <stddef.h>
#include <stdint.h>

#define CON_BYTE_MASK 0xFF
#define CON_SHORT_MASK 0xFFFF
#define CON_INT_MASK 0xFFFFFFFF

#define CON_BYTE_SHIFT 8
#define CON_SHORT_SHIFT 16
#define CON_TRIPPLE_BYTE_SHIFT 24
#define CON_INT_SHIFT 32

#define CON_BYTE_LENGTH 1
#define CON_SHORT_LENGTH 2
#define CON_INT_LENGTH 4
#define CON_LONG_LENGTH 8

#define CON_TRIPPLE_INT_LENGTH 12;


uint16_t CON_BytesToUnsigned16(uint8_t *bytes, uint8_t length);
uint32_t CON_BytesToUnsigned32(uint8_t *bytes, uint8_t length);
void CON_Unsigned16ToBytes(uint16_t input, uint8_t *bytes, uint8_t length);
void CON_Unsigned32ToBytes(uint32_t input, uint8_t *bytes, uint8_t length);
int16_t CON_BytesToSigned16(uint8_t *bytes, uint8_t length);
int32_t CON_BytesToSigned32(uint8_t *bytes, uint8_t length);
void CON_Signed16ToBytes(int16_t input, uint8_t *bytes, uint8_t length);
void CON_Signed32ToBytes(int32_t input, uint8_t *bytes, uint8_t length);

#endif /* INC_CONVERSION_H_ */
