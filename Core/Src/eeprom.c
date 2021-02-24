/*
 * eeprom.c
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
 *  Created on: Feb. 15, 2021
 *      Author: SG-O
 */


#include "eeprom.h"

uint8_t EEPROM_buffer[EEPROM_IO_BUFFER_SIZE];

uint32_t EEPROM_counterValue;
uint16_t EEPROM_counterSlot;
uint8_t EEPROM_initialized = 0;
uint64_t EEPROM_previousTotalFeeds = 0;

void EEPROM_Setup(I2C_HandleTypeDef *hi2c) {
	EEPROM_hi2c = hi2c;
	EEPROM_UpdateCounterSlot();
	EEPROM_CheckHeader();
}

void EEPROM_CheckForReInit() {
	if (HW_IsV1() != 1) return;
	uint32_t backupData = BACKUP_Read(BACKUP_REG_MESSAGE);
	if ((backupData && (BACKUP_MESSAGE_INIT_EEPROM)) == 0) return;
	backupData &= ~(BACKUP_MESSAGE_INIT_EEPROM);
	BACKUP_Write(BACKUP_REG_MESSAGE, backupData);
	EEPROM_WriteHeader();
	EEPROM_WriteDefaults();
	NVIC_SystemReset();
}

void HW_ReInit() {
	uint32_t backupData = BACKUP_Read(BACKUP_REG_MESSAGE);
	backupData |= BACKUP_MESSAGE_INIT_EEPROM;
	BACKUP_Write(BACKUP_REG_MESSAGE, backupData);
	NVIC_SystemReset();
}


uint16_t EEPROM_Read(uint16_t address, uint16_t length) {
	if (length > EEPROM_IO_BUFFER_SIZE) return 0;
	uint16_t leftInPage; // A page is EEPROM_PAGE_SIZE long. Reading outside the page boundary is not strictly illegal. This is just to be cautious.
	uint16_t read = 0; // Total bytes read.
	HAL_StatusTypeDef result;
	while (HAL_I2C_GetState(EEPROM_hi2c) != HAL_I2C_STATE_READY)
	{
		asm("NOP");
	}
	while (length > 0) {
		leftInPage = EEPROM_PAGE_SIZE - (address % EEPROM_PAGE_SIZE); // Calculate the number of bytes until the end of the page
		if (length < leftInPage) {
			leftInPage = length; // Don't read more bytes than required
		}
		result = HAL_I2C_Mem_Read(EEPROM_hi2c, EEPROM_ADDRESS << 1, address, I2C_MEMADD_SIZE_16BIT, &EEPROM_buffer[read], leftInPage, EEPROM_TIMEOUT); //Read the data into the buffer
		if (result != HAL_OK) return read; // If something went wrong return the actual number of bytes read.
		length -= leftInPage; // Remove the bytes read from the bytes to read
		address += leftInPage; // Move the address to the next read position
		read += leftInPage; // Add the bytes read to the total
		HAL_Delay(EEPROM_READ_DELAY); // Just to be safe a little delay
	}
	return read;
}

uint16_t EEPROM_Write(uint16_t address, uint16_t length) {
	if (length > EEPROM_IO_BUFFER_SIZE) return 0;
	if (EEPROM_initialized != 1) return 0;
	uint16_t leftInPage; // A page is EEPROM_PAGE_SIZE long. Writing outside the page boundary is not permitted.
	uint16_t written = 0; // Total bytes written.
	while (HAL_I2C_GetState(EEPROM_hi2c) != HAL_I2C_STATE_READY)
	{
		asm("NOP");
	}
	HAL_StatusTypeDef result;
	while (length > 0) {
		leftInPage = EEPROM_PAGE_SIZE - (address % EEPROM_PAGE_SIZE); // Calculate the number of bytes until the end of the page
		if (length < leftInPage) {
			leftInPage = length; // Don't write more bytes than required
		}
		result = HAL_I2C_Mem_Write(EEPROM_hi2c, EEPROM_ADDRESS << 1, address, I2C_MEMADD_SIZE_16BIT, &EEPROM_buffer[written], leftInPage, EEPROM_TIMEOUT); //Write the data from the buffer to the EEPROM
		if (result != HAL_OK) return written; // If something went wrong return the actual number of bytes written.
		length -= leftInPage; // Remove the bytes written from the bytes to write
		address += leftInPage; // Move the address to the next write position
		written += leftInPage;  // Add the bytes written to the total
		HAL_Delay(EEPROM_WRITE_DELAY); // This delay is required
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
	if (EEPROM_Read(address, CON_INT_LENGTH) != CON_INT_LENGTH) return def;
	uint32_t tmp;
	memcpy(&tmp, EEPROM_buffer, CON_INT_LENGTH);
	return tmp;
}

int32_t EEPROM_ReadInt32(uint16_t address, int32_t def) {
	if (EEPROM_Read(address, CON_INT_LENGTH) != CON_INT_LENGTH) return def;
	int32_t tmp;
	memcpy(&tmp, EEPROM_buffer, CON_INT_LENGTH);
	return tmp;
}

uint16_t EEPROM_ReadUint16(uint16_t address, uint16_t def) {
	if (EEPROM_Read(address, CON_SHORT_LENGTH) != CON_SHORT_LENGTH) return def;
	uint16_t tmp;
	memcpy(&tmp, EEPROM_buffer, CON_SHORT_LENGTH);
	return tmp;
}

int16_t EEPROM_ReadInt16(uint16_t address, int16_t def) {
	if (EEPROM_Read(address, CON_SHORT_LENGTH) != CON_SHORT_LENGTH) return def;
	int16_t tmp;
	memcpy(&tmp, EEPROM_buffer, CON_SHORT_LENGTH);
	return tmp;
}

uint8_t EEPROM_ReadUint8(uint16_t address, uint8_t def) {
	if (EEPROM_Read(address, CON_BYTE_LENGTH) != CON_BYTE_LENGTH) return def;
	uint8_t tmp;
	memcpy(&tmp, EEPROM_buffer, CON_BYTE_LENGTH);
	return tmp;
}

int8_t EEPROM_ReadInt8(uint16_t address, int8_t def) {
	if (EEPROM_Read(address, CON_BYTE_LENGTH) != CON_BYTE_LENGTH) return def;
	int8_t tmp;
	memcpy(&tmp, EEPROM_buffer, CON_BYTE_LENGTH);
	return tmp;
}

//--Writes--
uint16_t EEPROM_WriteString(uint16_t address, char *strg, uint16_t length) {
	memcpy(EEPROM_buffer, strg, length);
	if (EEPROM_Write(address, length) != length) {
		ERROR_SetError(ERROR_STORAGE_WRITE);
		return 0;
	}
	return length;
}

uint16_t EEPROM_WriteUint32(uint16_t address, uint32_t data) {
	memcpy(EEPROM_buffer, &data, CON_INT_LENGTH);
	if (EEPROM_Write(address, CON_INT_LENGTH) != CON_INT_LENGTH) {
		ERROR_SetError(ERROR_STORAGE_WRITE);
		return 0;
	}
	return CON_INT_LENGTH;
}

uint16_t EEPROM_WriteInt32(uint16_t address, int32_t data) {
	memcpy(EEPROM_buffer, &data, CON_INT_LENGTH);
	if (EEPROM_Write(address, CON_INT_LENGTH) != CON_INT_LENGTH) {
		ERROR_SetError(ERROR_STORAGE_WRITE);
		return 0;
	}
	return CON_INT_LENGTH;
}

uint16_t EEPROM_WriteUint16(uint16_t address, uint16_t data) {
	memcpy(EEPROM_buffer, &data, CON_SHORT_LENGTH);
	if (EEPROM_Write(address, CON_SHORT_LENGTH) != CON_SHORT_LENGTH) {
		ERROR_SetError(ERROR_STORAGE_WRITE);
		return 0;
	}
	return CON_SHORT_LENGTH;
}

uint16_t EEPROM_WriteInt16(uint16_t address, int16_t data) {
	memcpy(EEPROM_buffer, &data, CON_SHORT_LENGTH);
	if (EEPROM_Write(address, CON_SHORT_LENGTH) != CON_SHORT_LENGTH) {
		ERROR_SetError(ERROR_STORAGE_WRITE);
		return 0;
	}
	return CON_SHORT_LENGTH;
}

uint16_t EEPROM_WriteUint8(uint16_t address, uint8_t data) {
	memcpy(EEPROM_buffer, &data, CON_BYTE_LENGTH);
	if (EEPROM_Write(address, CON_BYTE_LENGTH) != CON_BYTE_LENGTH) {
		ERROR_SetError(ERROR_STORAGE_WRITE);
		return 0;
	}
	return CON_BYTE_LENGTH;
}

uint16_t EEPROM_WriteInt8(uint16_t address, int8_t data) {
	memcpy(EEPROM_buffer, &data, 1);
	if (EEPROM_Write(address, 1) != 1) {
		ERROR_SetError(ERROR_STORAGE_WRITE);
		return 0;
	}
	return CON_BYTE_LENGTH;
}

//--Counter--
void EEPROM_UpdateCounterSlot() {
	for (uint16_t i = 0; i < EEPROM_COUNTER_SLOTS; i++) {
		EEPROM_counterValue = EEPROM_ReadUint32(EEPROM_COUNTER_OFFSET + (i << EEPROM_COUNTER_ROW_OFFSET_SHIFT), EEPROM_MAXIMUM_WRITES);
		if (EEPROM_counterValue == CON_INT_MASK) EEPROM_counterValue = 0;
		if (EEPROM_counterValue < EEPROM_MAXIMUM_WRITES) {
			EEPROM_counterSlot = i;
			return;
		}
		EEPROM_previousTotalFeeds += EEPROM_ReadUint32(EEPROM_COUNTER_OFFSET + (i << EEPROM_COUNTER_ROW_OFFSET_SHIFT) + EEPROM_COUNTER_TOTAL_FEEDS_OFFSET, 0);
	}
	ERROR_SetError(ERROR_STORAGE_COUNTER_COUNT);
	EEPROM_counterSlot = EEPROM_COUNTER_SLOTS;
}

uint16_t EEPROM_CounterWriteUint32(uint16_t address, uint32_t data) {
	if (EEPROM_counterValue >= EEPROM_MAXIMUM_WRITES) EEPROM_UpdateCounterSlot();
	if (EEPROM_counterSlot >= EEPROM_COUNTER_SLOTS) return 0;
	EEPROM_counterValue++;
	address += EEPROM_counterSlot << EEPROM_COUNTER_ROW_OFFSET_SHIFT;
	if (EEPROM_WriteUint32(address & EEPROM_COUNTER_ROW_MASK, EEPROM_counterValue) != CON_INT_LENGTH) return 0;
	return EEPROM_WriteUint32(address, data);
}

uint16_t EEPROM_CounterWriteInt32(uint16_t address, int32_t data) {
	if (EEPROM_counterValue >= EEPROM_MAXIMUM_WRITES) EEPROM_UpdateCounterSlot();
	if (EEPROM_counterSlot >= EEPROM_COUNTER_SLOTS) return 0;
	EEPROM_counterValue++;
	address += EEPROM_counterSlot << EEPROM_COUNTER_ROW_OFFSET_SHIFT;
	if (EEPROM_WriteUint32(address & EEPROM_COUNTER_ROW_MASK, EEPROM_counterValue) != CON_INT_LENGTH) return 0;
	return EEPROM_WriteInt32(address, data);
}

uint16_t EEPROM_CounterCount(uint64_t totalFeeds, int32_t remainingParts) {
	if (EEPROM_counterValue >= EEPROM_MAXIMUM_WRITES) EEPROM_UpdateCounterSlot();
	if (EEPROM_counterSlot >= EEPROM_COUNTER_SLOTS) return 0;
	EEPROM_counterValue++;
	uint32_t totalFeedsRemaining = (uint32_t)((totalFeeds - EEPROM_previousTotalFeeds) & 0xFFFFFFFFL);
	uint16_t address = (EEPROM_counterSlot << EEPROM_COUNTER_ROW_OFFSET_SHIFT) + EEPROM_COUNTER_OFFSET;
	if (EEPROM_WriteUint32(address & EEPROM_COUNTER_ROW_MASK, EEPROM_counterValue) != CON_INT_LENGTH) return 0;
	EEPROM_WriteUint32(address + EEPROM_COUNTER_TOTAL_FEEDS_OFFSET, totalFeedsRemaining);
	return EEPROM_WriteInt32(address + EEPROM_COUNTER_REMAINING_PARTS_OFFSET, remainingParts);
}

uint32_t EEPROM_CounterReadUint32(uint16_t address, uint32_t def) {
	if (EEPROM_counterValue >= EEPROM_MAXIMUM_WRITES) EEPROM_UpdateCounterSlot();
	if (EEPROM_counterSlot >= EEPROM_COUNTER_SLOTS) return def;
	address += EEPROM_counterSlot << EEPROM_COUNTER_ROW_OFFSET_SHIFT;
	return EEPROM_ReadUint32(address, def);
}

int32_t EEPROM_CounterReadInt32(uint16_t address, int32_t def) {
	if (EEPROM_counterValue >= EEPROM_MAXIMUM_WRITES) EEPROM_UpdateCounterSlot();
	if (EEPROM_counterSlot >= EEPROM_COUNTER_SLOTS) return def;
	address += EEPROM_counterSlot << EEPROM_COUNTER_ROW_OFFSET_SHIFT;
	return EEPROM_ReadInt32(address, def);
}

uint64_t EEPROM_CounterCalcTotalFeeds(uint32_t readValue) {
	return EEPROM_previousTotalFeeds + readValue;
}

//--Util--
uint32_t EEPROM_CheckUpdateRowCounter(uint16_t address) {
	uint32_t value = EEPROM_ReadUint32(address & 0xFFF0, EEPROM_MAXIMUM_WRITES);
	if (value == CON_INT_MASK) value = 0;
	if (value >= EEPROM_MAXIMUM_WRITES) return EEPROM_MAXIMUM_WRITES;
	value++;
	EEPROM_WriteUint32(address, value);
	return value;
}

uint8_t EEPROM_WriteHeader() {
	EEPROM_initialized = 1;
	for(int i = 0; i < EEPROM_HEADER_CRC_OFFSET; i++) {
		EEPROM_buffer[i] = CON_BYTE_MASK;
	}
	uint32_t version = EEPROM_FORMAT_VERSION;
	memcpy(&EEPROM_buffer[EEPROM_HEADER_FORMAT_VERSION_OFFSET], &version, CON_INT_LENGTH);
	uint32_t id = HAL_GetUIDw0();
	memcpy(&EEPROM_buffer[EEPROM_HEADER_DEVICE_ID_0_OFFSET], &id, CON_INT_LENGTH);
	id = HAL_GetUIDw1();
	memcpy(&EEPROM_buffer[EEPROM_HEADER_DEVICE_ID_1_OFFSET], &id, CON_INT_LENGTH);
	id = HAL_GetUIDw2();
	memcpy(&EEPROM_buffer[EEPROM_HEADER_DEVICE_ID_2_OFFSET], &id, CON_INT_LENGTH);
	uint32_t crc = CRC_Calculate32(EEPROM_buffer, EEPROM_HEADER_CRC_OFFSET);
	memcpy(&EEPROM_buffer[EEPROM_HEADER_CRC_OFFSET], &crc, CON_INT_LENGTH);
	if (EEPROM_Write(EEPROM_HEADER_OFFSET, EEPROM_HEADER_LENGTH) == EEPROM_HEADER_LENGTH) {
		EEPROM_initialized = 1;
		return EEPROM_initialized;
	}
	EEPROM_initialized = 0;
	ERROR_SetError(ERROR_STORAGE_INIT);
	return EEPROM_initialized;
}

uint8_t EEPROM_WriteDefaults() {
	if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_0_OFFSET) < EEPROM_MAXIMUM_WRITES) {
		EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_PART_PITCH_OFFSET, CONFIG_DEFAULT_PART_PITCH);
		EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_FEED_SPEED_OFFSET, CONFIG_DEFAULT_FEED_SPEED);
		EEPROM_WriteUint16(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LOW_PARTS_WARNING_OFFSET, CONFIG_DEFAULT_LOW_PARTS_WARN);
	} else {
		ERROR_SetError(ERROR_STORAGE_CONFIG_COUNT);
	}
	if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_1_OFFSET) < EEPROM_MAXIMUM_WRITES) {
		EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_DISPLAY_BRIGHTNESS_OFFSET, CONFIG_DEFAULT_DISPLAY_BRIGHTNESS);
		EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_MOTOR_DIRECTION_OFFSET, CONFIG_DEFAULT_MOTOR_DIRECTION);
		EEPROM_WriteUint16(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_MOTOR_SLOWDOWN_DELAY_OFFSET, CONFIG_DEFAULT_MOTOR_SLOWDOWN_DELAY);
	} else {
		ERROR_SetError(ERROR_STORAGE_CONFIG_COUNT);
	}
	if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_2_OFFSET) < EEPROM_MAXIMUM_WRITES) {
		EEPROM_WriteInt32(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_TOTAL_PARTS_OFFSET, CONFIG_DEFAULT_TOTAL_PARTS);
	} else {
		ERROR_SetError(ERROR_STORAGE_CONFIG_COUNT);
	}
	EEPROM_UpdateCounterSlot();
	EEPROM_CounterWriteUint32(EEPROM_COUNTER_OFFSET + EEPROM_COUNTER_TOTAL_FEEDS_OFFSET, 0);
	EEPROM_CounterWriteInt32(EEPROM_COUNTER_OFFSET + EEPROM_COUNTER_REMAINING_PARTS_OFFSET, CONFIG_DEFAULT_REMAINING_PARTS);
	if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_3_OFFSET) < EEPROM_MAXIMUM_WRITES) {
		char id[EEPROM_CONFIG_SHORT_ID_LENGTH];
		for (int i = 0; i < EEPROM_CONFIG_SHORT_ID_LENGTH; i++) {
			id[i] = 0;
		}
		uint32_t devID = HAL_GetUIDw0() ^ HAL_GetUIDw1() ^ HAL_GetUIDw2();
		uint8_t devIDByte[CON_INT_LENGTH];
		*(uint32_t*)&devIDByte = devID;
		encode_ascii85(devIDByte, CON_INT_LENGTH, id, EEPROM_CONFIG_SHORT_ID_LENGTH);
		EEPROM_WriteString(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_SHORT_ID_OFFSET, id, EEPROM_CONFIG_SHORT_ID_LENGTH);
		EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_SHORT_ID_FILLER_0_OFFSET, 0);
		EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_SHORT_ID_FILLER_1_OFFSET, 0);
	} else {
		ERROR_SetError(ERROR_STORAGE_CONFIG_COUNT);
	}
	if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LONG_ID_WRITES_OFFSET) < EEPROM_MAXIMUM_WRITES) {
		for (int i = 0; i < EEPROM_CONFIG_LONG_ID_LENGTH; i++) {
			EEPROM_buffer[i] = 0;
			memcpy(EEPROM_buffer, CONFIG_DEFAULT_LONG_PARTS_ID, sizeof(CONFIG_DEFAULT_LONG_PARTS_ID));
		}
		EEPROM_Write(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LONG_ID_OFFSET, EEPROM_CONFIG_LONG_ID_LENGTH);
		EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LONG_ID_FILLER_OFFSET, 0);
	} else {
		ERROR_SetError(ERROR_STORAGE_CONFIG_COUNT);
	}
	return EEPROM_initialized;
}

uint8_t EEPROM_CheckHeader() {
	if (EEPROM_Read(EEPROM_HEADER_OFFSET, EEPROM_HEADER_LENGTH) != EEPROM_HEADER_LENGTH) {
		EEPROM_initialized = 0;
		return EEPROM_initialized;
	}
	uint32_t version;
	memcpy(&version, &EEPROM_buffer[EEPROM_HEADER_FORMAT_VERSION_OFFSET], CON_INT_LENGTH);
	if (version == CON_INT_MASK) {
		EEPROM_WriteHeader();
		return EEPROM_WriteDefaults();
	} else if (version != EEPROM_FORMAT_VERSION) {
		EEPROM_initialized = 0;
		ERROR_SetError(ERROR_STORAGE_HEADER);
		return EEPROM_initialized;
	}
	uint32_t crc;
	memcpy(&crc, &EEPROM_buffer[EEPROM_HEADER_CRC_OFFSET], CON_INT_LENGTH);
	if (CRC_Calculate32(EEPROM_buffer, EEPROM_HEADER_CRC_OFFSET) != crc) {
		EEPROM_initialized = 0;
		ERROR_SetError(ERROR_STORAGE_HEADER);
		return EEPROM_initialized;
	}
	uint32_t id;
	memcpy(&id, &EEPROM_buffer[EEPROM_HEADER_DEVICE_ID_0_OFFSET], CON_INT_LENGTH);
	if (id != HAL_GetUIDw0()) {
		EEPROM_initialized = 0;
		ERROR_SetError(ERROR_STORAGE_HEADER);
		return EEPROM_initialized;
	}
	memcpy(&id, &EEPROM_buffer[EEPROM_HEADER_DEVICE_ID_1_OFFSET], CON_INT_LENGTH);
	if (id != HAL_GetUIDw1()) {
		EEPROM_initialized = 0;
		ERROR_SetError(ERROR_STORAGE_HEADER);
		return EEPROM_initialized;
	}
	memcpy(&id, &EEPROM_buffer[EEPROM_HEADER_DEVICE_ID_2_OFFSET], CON_INT_LENGTH);
	if (id != HAL_GetUIDw2()) {
		EEPROM_initialized = 0;
		ERROR_SetError(ERROR_STORAGE_HEADER);
		return EEPROM_initialized;
	}
	EEPROM_initialized = 1;
	return EEPROM_initialized;
}


