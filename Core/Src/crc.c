/*
 * crc.c
 *
 *  Created on: Jan. 24, 2021
 *      Author: SG-O
 */

#include "crc.h"

uint32_t preConditioned[64];
uint8_t CRC_crcH;

void CRC_Start(CRC_HandleTypeDef *hcrc) {
	CRC_hcrc = hcrc;
}

uint8_t CRC_Condition(uint8_t *input, uint8_t inputLength) {
	uint8_t i = 0;
	uint8_t rLengthCalculate = inputLength / 4;
	if ((inputLength % 4) > 0) rLengthCalculate += 1;
	for (uint8_t j = 0; j < rLengthCalculate; j++) {
		preConditioned[j] = 0;
	}
	while (i < inputLength) {
		preConditioned[i / 4] = preConditioned[i / 4] << 8;
		preConditioned[i / 4] |= input[i];
		i++;
	}
	while ((i % 4) > 0) {
		preConditioned[i / 4] = preConditioned[i / 4] << 8;
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
	uint8_t res = crc & 0xFF;
	CRC_crcH = (crc >> 8) & 0xFF;
	return res;
}
