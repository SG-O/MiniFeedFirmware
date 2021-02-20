/*
 * persist.c
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
 *  Created on: Jan 20, 2021
 *      Author: SG-O
 */

#include "persist.h"
#include "ascii85.h"

uint64_t PERSIST_totalFeeds = 0;

uint8_t PERSIST_partPitch;
uint8_t PERSIST_feedSpeed;
uint8_t PERSIST_displayBrightness;
uint8_t PERSIST_motorDir;
uint8_t PERSIST_i2cAddress;
uint16_t PERSIST_motorSlowDelay;

int32_t PERSIST_totalParts;
int32_t PERSIST_remainingParts;
uint16_t PERSIST_lowPartWarn;
char PERSIST_shortPartID[PERSISt_SHORT_ID_LENGTH];
char PERSIST_longPartID[PERSISt_LONG_ID_LENGTH];

void PERSIST_LoadDefaults() {
	PERSIST_i2cAddress = PERSIST_INIT_I2C_ADDRESS;

	PERSIST_partPitch = CONFIG_DEFAULT_PART_PITCH;
	if (PERSIST_partPitch < HW_GetMinFeed()) PERSIST_partPitch = HW_GetMinFeed();
	PERSIST_feedSpeed = CONFIG_DEFAULT_FEED_SPEED;
	PERSIST_displayBrightness = CONFIG_DEFAULT_DISPLAY_BRIGHTNESS;
	PERSIST_motorDir = CONFIG_DEFAULT_MOTOR_DIRECTION;
	PERSIST_motorSlowDelay = CONFIG_DEFAULT_MOTOR_SLOWDOWN_DELAY;
	PERSIST_totalParts = CONFIG_DEFAULT_TOTAL_PARTS;
	PERSIST_remainingParts = CONFIG_DEFAULT_REMAINING_PARTS;
	PERSIST_lowPartWarn = CONFIG_DEFAULT_LOW_PARTS_WARN;
	PERSIST_SetLongPartID(CONFIG_DEFAULT_LONG_PARTS_ID, sizeof(CONFIG_DEFAULT_LONG_PARTS_ID));
	char id[PERSISt_SHORT_ID_LENGTH];
	uint32_t devID = HAL_GetUIDw0() ^ HAL_GetUIDw1() ^ HAL_GetUIDw2();
	uint8_t devIDByte[CON_INT_LENGTH];
	*(uint32_t*)&devIDByte = devID;
	encode_ascii85(devIDByte, CON_INT_LENGTH, id, PERSISt_SHORT_ID_LENGTH);
	PERSIST_SetShortPartID(id, PERSISt_SHORT_ID_LENGTH - 1);
}

void PERSIST_LoadFromStorage() {
	PERSIST_partPitch = EEPROM_ReadUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_PART_PITCH_OFFSET, CONFIG_DEFAULT_PART_PITCH);
	if (PERSIST_partPitch < HW_GetMinFeed()) PERSIST_partPitch = HW_GetMinFeed();
	PERSIST_feedSpeed = EEPROM_ReadUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_FEED_SPEED_OFFSET, CONFIG_DEFAULT_FEED_SPEED);
	PERSIST_displayBrightness = EEPROM_ReadUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_DISPLAY_BRIGHTNESS_OFFSET, CONFIG_DEFAULT_DISPLAY_BRIGHTNESS);
	PERSIST_motorDir = EEPROM_ReadUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_MOTOR_DIRECTION_OFFSET, CONFIG_DEFAULT_MOTOR_DIRECTION);
	PERSIST_motorSlowDelay = EEPROM_ReadUint16(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_MOTOR_SLOWDOWN_DELAY_OFFSET, CONFIG_DEFAULT_MOTOR_SLOWDOWN_DELAY);
	PERSIST_totalParts = EEPROM_ReadInt32(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_TOTAL_PARTS_OFFSET, CONFIG_DEFAULT_TOTAL_PARTS);
	PERSIST_lowPartWarn = EEPROM_ReadUint16(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LOW_PARTS_WARNING_OFFSET, CONFIG_DEFAULT_LOW_PARTS_WARN);

	uint32_t readTotalFeeds = EEPROM_CounterReadInt32(EEPROM_COUNTER_OFFSET + EEPROM_COUNTER_TOTAL_FEEDS_OFFSET, 0);
	PERSIST_remainingParts = EEPROM_CounterReadInt32(EEPROM_COUNTER_OFFSET + EEPROM_COUNTER_REMAINING_PARTS_OFFSET, CONFIG_DEFAULT_REMAINING_PARTS);
	PERSIST_totalFeeds = EEPROM_CounterCalcTotalFeeds(readTotalFeeds);

	EEPROM_ReadString(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_SHORT_ID_OFFSET, PERSIST_shortPartID, PERSISt_SHORT_ID_LENGTH);
	EEPROM_ReadString(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LONG_ID_OFFSET, PERSIST_longPartID, PERSISt_LONG_ID_LENGTH);
}

void PERSIST_Setup(I2C_HandleTypeDef *hi2c){
	PERSIST_hi2c = hi2c;
	PERSIST_LoadDefaults();
	if (HW_IsV1() == 1) {
		EEPROM_Setup(hi2c);
		uint8_t eepromOk = EEPROM_CheckHeader();
		if (eepromOk == 1) {
			PERSIST_LoadFromStorage();
		} else {
			ERROR_SetError(ERROR_INITIALISATION_FAILED);
		}
	}
}

void PERSIST_SetI2CAddress(uint8_t address) {
	if (address < PERSIST_MIN_I2C_ADDRESS || address > PERSIST_MAX_I2C_ADDRESS) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		address = PERSIST_INIT_I2C_ADDRESS;
	}
	PERSIST_i2cAddress = address;
}

void PERSIST_SetPartPitch(uint8_t partPitch) {
	if (partPitch < HW_GetMinFeed()) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			partPitch = HW_GetMinFeed();
		}
	PERSIST_partPitch = partPitch;
	if (HW_IsV1() == 1) {
		if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_0_OFFSET) < EEPROM_MAXIMUM_WRITES) {
			EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_PART_PITCH_OFFSET, partPitch);
		}
	}
}

void PERSIST_SetFeedSpeed(uint8_t feedSpeed) {
	if (feedSpeed < 1) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		feedSpeed = 1;
	}
	PERSIST_feedSpeed = feedSpeed;
	if (HW_IsV1() == 1) {
		if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_0_OFFSET) < EEPROM_MAXIMUM_WRITES) {
			EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_FEED_SPEED_OFFSET, feedSpeed);
		}
	}
}

void PERSIST_SetDisplayBrightness(uint8_t displayBrightness) {
	if (displayBrightness < 1) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			displayBrightness = 1;
		}
	PERSIST_displayBrightness = displayBrightness;
	if (HW_IsV1() == 1) {
		if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_1_OFFSET) < EEPROM_MAXIMUM_WRITES) {
			EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_DISPLAY_BRIGHTNESS_OFFSET, displayBrightness);
		}
	}
}

void PERSIST_SetMotorDir(uint8_t motorDir) {
	if (motorDir > 1) {
		motorDir = 1;
		ERROR_SetError(ERROR_INVALID_INPUT);
	}
	PERSIST_motorDir = motorDir;
	if (HW_IsV1() == 1) {
		if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_1_OFFSET) < EEPROM_MAXIMUM_WRITES) {
			EEPROM_WriteUint8(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_MOTOR_DIRECTION_OFFSET, motorDir);
		}
	}
}

void PERSIST_SetMotorSlowDelay(uint16_t motorSlowDelay) {
	PERSIST_motorSlowDelay = motorSlowDelay;
	if (HW_IsV1() == 1) {
		if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_1_OFFSET) < EEPROM_MAXIMUM_WRITES) {
			EEPROM_WriteUint16(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_MOTOR_SLOWDOWN_DELAY_OFFSET, motorSlowDelay);
		}
	}
}

void PERSIST_SetTotalParts(int32_t totalParts) {
	PERSIST_totalParts = totalParts;
	if (HW_IsV1() == 1) {
		if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_2_OFFSET) < EEPROM_MAXIMUM_WRITES) {
			EEPROM_WriteInt32(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_TOTAL_PARTS_OFFSET, totalParts);
		}
	}
}

void PERSIST_SetRemainingParts(int32_t remainingParts) {
	PERSIST_remainingParts = remainingParts;
	if (HW_IsV1() == 1) {
		EEPROM_CounterWriteInt32(EEPROM_COUNTER_OFFSET + EEPROM_COUNTER_REMAINING_PARTS_OFFSET, remainingParts);
	}
}

void PERSIST_SetLowPartWarn(uint16_t lowPartWarn) {
	PERSIST_lowPartWarn = lowPartWarn;
	if (EEPROM_CheckUpdateRowCounter(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_ROW_WRITE_0_OFFSET) < EEPROM_MAXIMUM_WRITES) {
		EEPROM_WriteUint16(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LOW_PARTS_WARNING_OFFSET, lowPartWarn);
	}
}

void PERSIST_SetShortPartID(char* shortPartID, uint8_t length) {
	if (length < 1) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		return;
	}
	if (length > PERSISt_SHORT_ID_LENGTH) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		return;
	}
	if (length == PERSISt_SHORT_ID_LENGTH) {
		if (shortPartID[PERSISt_SHORT_ID_LENGTH - 1] != 0x00) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			return;
		}
	}
	int i = 0;
	while (i < length) {
		PERSIST_shortPartID[i] = shortPartID[i];
		i++;
	}
	while (i < PERSISt_SHORT_ID_LENGTH) {
		PERSIST_shortPartID[i] = 0x00;
		i++;
	}
	if (HW_IsV1() == 1) {
		EEPROM_WriteString(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_SHORT_ID_OFFSET, PERSIST_shortPartID, PERSISt_SHORT_ID_LENGTH);
	}
}

void PERSIST_SetLongPartID(char* longPartID, uint8_t length) {
	if (length < 1) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			return;
		}
		if (length > PERSISt_LONG_ID_LENGTH) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			return;
		}
		if (length == PERSISt_LONG_ID_LENGTH) {
			if (longPartID[PERSISt_LONG_ID_LENGTH - 1] != 0x00) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				return;
			}
		}
		int i = 0;
		while (i < length) {
			PERSIST_longPartID[i] = longPartID[i];
			i++;
		}
		while (i < PERSISt_LONG_ID_LENGTH) {
			PERSIST_longPartID[i] = 0x00;
			i++;
		}
		if (HW_IsV1() == 1) {
			EEPROM_WriteString(EEPROM_CONFIG_0_OFFSET + EEPROM_CONFIG_LONG_ID_OFFSET, PERSIST_longPartID, PERSISt_LONG_ID_LENGTH);
		}
}

uint8_t PERSIST_GetI2CAddress() {
	return PERSIST_i2cAddress;
}

uint8_t PERSIST_GetPartPitch() {
	return PERSIST_partPitch;
}

uint8_t PERSIST_GetFeedSpeed() {
	return PERSIST_feedSpeed;
}

uint8_t PERSIST_GetDisplayBrightness() {
	return PERSIST_displayBrightness;
}

uint8_t PERSIST_GetMotorDir() {
	if (PERSIST_motorDir > 0) return 1;
	return 0;
}

uint16_t PERSIST_GetMotorSlowDelay() {
	return PERSIST_motorSlowDelay;
}

int32_t PERSIST_GetTotalParts() {
	return PERSIST_totalParts;
}

int32_t PERSIST_GetRemainingParts() {
	return PERSIST_remainingParts;
}

uint16_t PERSIST_GetLowPartWarn() {
	return PERSIST_lowPartWarn;
}

char* PERSIST_GetShortPartID() {
	return PERSIST_shortPartID;
}

char* PERSIST_GetLongPartID() {
	return PERSIST_longPartID;
}

uint64_t PERSIST_GetTotalFeeds() {
	return PERSIST_totalFeeds;
}

void PERSIST_DecRemainingParts() {
	PERSIST_remainingParts--;
	PERSIST_totalFeeds++;
	if (HW_IsV1() == 1) {
		EEPROM_CounterCount(PERSIST_totalFeeds, PERSIST_remainingParts);
	}
}
