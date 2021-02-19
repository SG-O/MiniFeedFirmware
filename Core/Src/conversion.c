/*
 * conversion.c
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

#include "conversion.h"

uint16_t CON_BytesToUnsigned16(uint8_t *bytes, uint8_t length) {
	if (length < CON_SHORT_LENGTH) return 0;
	uint16_t result = bytes[0]; // @suppress("Avoid magic numbers")
	result = result << CON_BYTE_SHIFT;
	result |= bytes[1];
	return result;
}

uint32_t CON_BytesToUnsigned32(uint8_t *bytes, uint8_t length) {
	if (length < CON_INT_LENGTH) return 0;
	uint32_t result = bytes[0]; // @suppress("Avoid magic numbers")
	result = result << CON_BYTE_SHIFT;
	result |= bytes[1]; // @suppress("Avoid magic numbers")
	result = result << CON_BYTE_SHIFT;
	result |= bytes[2]; // @suppress("Avoid magic numbers")
	result = result << CON_BYTE_SHIFT;
	result |= bytes[3]; // @suppress("Avoid magic numbers")
	return result;
}

void CON_Unsigned16ToBytes(uint16_t input, uint8_t *bytes, uint8_t length) {
	if (length < CON_SHORT_LENGTH) return;
	bytes[1] = input & CON_BYTE_MASK; // @suppress("Avoid magic numbers")
	bytes[0] = (input >> CON_BYTE_SHIFT) & CON_BYTE_MASK; // @suppress("Avoid magic numbers")
}

void CON_Unsigned32ToBytes(uint32_t input, uint8_t *bytes, uint8_t length) {
	if (length < CON_INT_LENGTH) return;
	bytes[3] = input & CON_BYTE_MASK; // @suppress("Avoid magic numbers")
	bytes[2] = (input >> CON_BYTE_SHIFT) & CON_BYTE_MASK; // @suppress("Avoid magic numbers")
	bytes[1] = (input >> CON_SHORT_SHIFT) & CON_BYTE_MASK; // @suppress("Avoid magic numbers")
	bytes[0] = (input >> CON_TRIPPLE_BYTE_SHIFT) & CON_BYTE_MASK; // @suppress("Avoid magic numbers")
}

int16_t CON_BytesToSigned16(uint8_t *bytes, uint8_t length) {
	return (int16_t) CON_BytesToUnsigned16(bytes, length);
}

int32_t CON_BytesToSigned32(uint8_t *bytes, uint8_t length) {
	return (int32_t) CON_BytesToUnsigned32(bytes, length);
}

void CON_Signed16ToBytes(int16_t input, uint8_t *bytes, uint8_t length) {
	CON_Unsigned16ToBytes((uint16_t) input, bytes, length);
}

void CON_Signed32ToBytes(int32_t input, uint8_t *bytes, uint8_t length) {
	CON_Unsigned32ToBytes((uint32_t) input, bytes, length);
}
