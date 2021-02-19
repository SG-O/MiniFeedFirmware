/*
 * crc.c
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

#include "crc.h"

uint32_t preConditioned[CRC_CONDITIONED_LENGTH];
uint8_t CRC_crcH;

void CRC_Start(CRC_HandleTypeDef *hcrc) {
	CRC_hcrc = hcrc;
}

uint8_t CRC_Condition(uint8_t *input, uint8_t inputLength) {
	uint8_t i = 0;
	uint8_t rLengthCalculate = inputLength / 4;
	if ((inputLength & 0b11) > 0) rLengthCalculate += 1; // @suppress("Avoid magic numbers")
	for (uint8_t j = 0; j < rLengthCalculate; j++) {
		preConditioned[j] = 0;
	}
	while (i < inputLength) {
		preConditioned[i >> 2] = preConditioned[i >> 2] << CON_BYTE_SHIFT;
		preConditioned[i >> 2] |= input[i];
		i++;
	}
	while ((i & 0b11) > 0) { // @suppress("Avoid magic numbers")
		preConditioned[i >> 2] = preConditioned[i >> 2] << CON_BYTE_SHIFT;
		i++;
	}
	return rLengthCalculate;
}

uint32_t CRC_Calculate(uint32_t *toCalculate, uint8_t length) {
	uint32_t crc = ~HAL_CRC_Calculate(CRC_hcrc, toCalculate, length);
	return crc;
}

uint8_t CRC_Get16H() {
	return CRC_crcH;
}

uint8_t CRC_Calculate8L(uint8_t *input, uint8_t inputLength) {
	if (inputLength < 1) return 0;
	uint8_t CRCLength = CRC_Condition(input, inputLength);
	uint32_t crc = CRC_Calculate(preConditioned, CRCLength);
	uint8_t res = crc & CON_BYTE_MASK;
	CRC_crcH = (crc >> CON_BYTE_SHIFT) & CON_BYTE_MASK;
	return res;
}

uint32_t CRC_Calculate32(uint8_t *input, uint8_t inputLength) {
	if (inputLength < 1) return 0;
	uint8_t CRCLength = CRC_Condition(input, inputLength);
	return CRC_Calculate(preConditioned, CRCLength);
}
