/*
 * eeprom.c
 *
 *  Created on: Feb. 15, 2021
 *      Author: SG-O
 */


#include "eeprom.h"

uint8_t EEPROM_buffer[128];

uint32_t EEPROM_counterValue;
uint16_t EEPROM_counterSlot;

void EEPROM_Setup(I2C_HandleTypeDef *hi2c) {
	EEPROM_hi2c = hi2c;
	EEPROM_UpdateCounterSlot();
}


uint16_t EEPROM_Read(uint16_t address, uint16_t length) {
	if (length > 128) return 0;
	uint16_t leftInPage;
	uint16_t read = 0;
	HAL_StatusTypeDef result;
	while (length > 0) {
		leftInPage = EEPROM_PAGE_SIZE - (address % EEPROM_PAGE_SIZE);
		if (length < leftInPage) {
			leftInPage = length;
		}
		result = HAL_I2C_Mem_Read(EEPROM_hi2c, EEPROM_ADDRESS << 1, address, I2C_MEMADD_SIZE_16BIT, &EEPROM_buffer[read], leftInPage, EEPROM_TIMEOUT);
		if (result != HAL_OK) return read;
		length -= leftInPage;
		address += leftInPage;
		read += leftInPage;
		HAL_Delay(EEPROM_READ_DELAY);
	}
	return read;
}

uint16_t EEPROM_Write(uint16_t address, uint16_t length) {
	if (length > 128) return 0;
	uint16_t leftInPage;
	uint16_t written = 0;
	HAL_StatusTypeDef result;
	while (length > 0) {
		leftInPage = EEPROM_PAGE_SIZE - (address % EEPROM_PAGE_SIZE);
		if (length < leftInPage) {
			leftInPage = length;
		}
		result = HAL_I2C_Mem_Write(EEPROM_hi2c, EEPROM_ADDRESS << 1, address, I2C_MEMADD_SIZE_16BIT, &EEPROM_buffer[written], leftInPage, EEPROM_TIMEOUT);
		if (result != HAL_OK) return written;
		length -= leftInPage;
		address += leftInPage;
		written += leftInPage;
		HAL_Delay(EEPROM_WRITE_DELAY);
	}
	return written;
}

//--Reads--
uint16_t EEPROM_ReadString(uint16_t address, char *strg, uint16_t length) {
	if (EEPROM_Read(address, length) != length) return 0;
	memcpy(strg, EEPROM_buffer, length);
	return length;
}

uint32_t EEPROM_ReadUint32(uint16_t address, uint32_t def) {
	if (EEPROM_Read(address, 4) != 4) return def;
	uint32_t tmp;
	memcpy(&tmp, EEPROM_buffer, 4);
	return tmp;
}

int32_t EEPROM_ReadInt32(uint16_t address, int32_t def) {
	if (EEPROM_Read(address, 4) != 4) return def;
	int32_t tmp;
	memcpy(&tmp, EEPROM_buffer, 4);
	return tmp;
}

uint16_t EEPROM_ReadUint16(uint16_t address, uint16_t def) {
	if (EEPROM_Read(address, 2) != 2) return def;
	uint16_t tmp;
	memcpy(&tmp, EEPROM_buffer, 2);
	return tmp;
}

int16_t EEPROM_ReadInt16(uint16_t address, int16_t def) {
	if (EEPROM_Read(address, 2) != 2) return def;
	int16_t tmp;
	memcpy(&tmp, EEPROM_buffer, 2);
	return tmp;
}

uint8_t EEPROM_ReadUint8(uint16_t address, uint8_t def) {
	if (EEPROM_Read(address, 1) != 1) return def;
	uint8_t tmp;
	memcpy(&tmp, EEPROM_buffer, 1);
	return tmp;
}

int8_t EEPROM_ReadInt8(uint16_t address, int8_t def) {
	if (EEPROM_Read(address, 1) != 1) return def;
	int8_t tmp;
	memcpy(&tmp, EEPROM_buffer, 1);
	return tmp;
}

//--Writes--
uint16_t EEPROM_WriteString(uint16_t address, char *strg, uint16_t length) {
	memcpy(EEPROM_buffer, strg, length);
	if (EEPROM_Read(address, length) != length) return 0;
	return length;
}

uint16_t EEPROM_WriteUint32(uint16_t address, uint32_t data) {
	memcpy(EEPROM_buffer, &data, 4);
	if (EEPROM_Read(address, 4) != 4) return 0;
	return 4;
}

uint16_t EEPROM_WriteInt32(uint16_t address, int32_t data) {
	memcpy(EEPROM_buffer, &data, 4);
	if (EEPROM_Read(address, 4) != 4) return 0;
	return 4;
}

uint16_t EEPROM_WriteUint16(uint16_t address, uint16_t data) {
	memcpy(EEPROM_buffer, &data, 2);
	if (EEPROM_Read(address, 2) != 2) return 0;
	return 2;
}

uint16_t EEPROM_WriteInt16(uint16_t address, int16_t data) {
	memcpy(EEPROM_buffer, &data, 2);
	if (EEPROM_Read(address, 2) != 2) return 0;
	return 2;
}

uint16_t EEPROM_WriteUint8(uint16_t address, uint8_t data) {
	memcpy(EEPROM_buffer, &data, 1);
	if (EEPROM_Read(address, 1) != 1) return 0;
	return 1;
}

uint16_t EEPROM_WriteInt8(uint16_t address, int8_t data) {
	memcpy(EEPROM_buffer, &data, 1);
	if (EEPROM_Read(address, 1) != 1) return 0;
	return 1;
}

//--Counter--
void EEPROM_UpdateCounterSlot() {
	for (uint16_t i = 0; i < EEPROM_COUNTER_SLOTS; i++) {
		EEPROM_counterValue = EEPROM_ReadUint32(EEPROM_COUNTER_OFFSET + (i << 4), 1000000);
		if (EEPROM_counterValue == 0xFFFFFFFF) EEPROM_counterValue = 0;
		if (EEPROM_counterValue < 1000000) {
			EEPROM_counterSlot = i;
			return;
		}
	}
	EEPROM_counterSlot = EEPROM_COUNTER_SLOTS;
}

uint16_t EEPROM_CounterWriteUint32(uint16_t address, uint32_t data) {
	if (EEPROM_counterValue >= 1000000) EEPROM_UpdateCounterSlot();
	if (EEPROM_counterSlot >= EEPROM_COUNTER_SLOTS) return 0;
	EEPROM_counterValue++;
	address += EEPROM_counterSlot << 4;
	if (EEPROM_WriteUint32(address & 0xFFF0, EEPROM_counterValue) != 4) return 0;
	return EEPROM_WriteUint32(address, data);
}

uint16_t EEPROM_CounterWriteInt32(uint16_t address, int32_t data) {
	if (EEPROM_counterValue >= 1000000) EEPROM_UpdateCounterSlot();
		if (EEPROM_counterSlot >= EEPROM_COUNTER_SLOTS) return 0;
		EEPROM_counterValue++;
		address += EEPROM_counterSlot << 4;
		if (EEPROM_WriteUint32(address & 0xFFF0, EEPROM_counterValue) != 4) return 0;
		return EEPROM_WriteInt32(address, data);
}

//--Util--
uint32_t EEPROM_CheckUpdateRowCounter(uint16_t address) {
	uint32_t value = EEPROM_ReadUint32(address & 0xFFF0, 1000000);
	if (value == 0xFFFFFFFF) value = 0;
	if (value >= 1000000) return 1000000;
	value++;
	EEPROM_WriteUint32(address, value);
	return value;
}
