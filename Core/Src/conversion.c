/*
 * conversion.c
 *
 *  Created on: Jan. 24, 2021
 *      Author: SG-O
 */

#include "conversion.h"

uint16_t CON_BytesToUnsigned16(uint8_t *bytes, uint8_t length) {
	if (length < 2) return 0;
	uint16_t result = bytes[0];
	result = result << 8;
	result |= bytes[1];
	return result;
}

uint32_t CON_BytesToUnsigned32(uint8_t *bytes, uint8_t length) {
	if (length < 4) return 0;
	uint32_t result = bytes[0];
	result = result << 8;
	result |= bytes[1];
	result = result << 8;
	result |= bytes[2];
	result = result << 8;
	result |= bytes[3];
	return result;
}

void CON_Unsigned16ToBytes(uint16_t input, uint8_t *bytes, uint8_t length) {
	if (length < 2) return;
	bytes[1] = input & 0xFF;
	bytes[0] = (input >> 8) & 0xFF;
}

void CON_Unsigned32ToBytes(uint32_t input, uint8_t *bytes, uint8_t length) {
	if (length < 4) return;
	bytes[3] = input & 0xFF;
	bytes[2] = (input >> 8) & 0xFF;
	bytes[1] = (input >> 16) & 0xFF;
	bytes[0] = (input >> 24) & 0xFF;
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
