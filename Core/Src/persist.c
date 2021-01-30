/*
 * persist.c
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
char PERSIST_shortPartID[6];
char PERSIST_longPartID[127];

void PERSIST_LoadDefaults() {
	PERSIST_i2cAddress = 0x50;

	PERSIST_partPitch = CONFIG_DEFAULT_PART_PITCH;
	PERSIST_feedSpeed = CONFIG_DEFAULT_FEED_SPEED;
	PERSIST_displayBrightness = CONFIG_DEFAULT_DISPLAY_BRIGHTNESS;
	PERSIST_motorDir = CONFIG_DEFAULT_MOTOR_DIRECTION;
	PERSIST_motorSlowDelay = CONFIG_DEFAULT_MOTOR_SLOWDOWN_DELAY;
	PERSIST_totalParts = CONFIG_DEFAULT_TOTAL_PARTS;
	PERSIST_remainingParts = CONFIG_DEFAULT_REMAINING_PARTS;
	PERSIST_lowPartWarn = CONFIG_DEFAULT_LOW_PARTS_WARN;
	PERSIST_SetLongPartID(CONFIG_DEFAULT_LONG_PARTS_ID, sizeof(CONFIG_DEFAULT_LONG_PARTS_ID));
	char id[6];
	uint32_t devID = HAL_GetUIDw0() ^ HAL_GetUIDw1() ^ HAL_GetUIDw2();
	uint8_t devIDByte[4];
	*(uint32_t*)&devIDByte = devID;
	encode_ascii85(devIDByte, 4, id, 6);
	PERSIST_SetShortPartID(id, 5);
}

void PERSIST_Setup(I2C_HandleTypeDef *hi2c){
	PERSIST_hi2c = hi2c;
	PERSIST_LoadDefaults();
}

void PERSIST_SetI2CAddress(uint8_t address) {
	if (address < 0x10 || address > 0x50) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		address = 0x50;
	}
	PERSIST_i2cAddress = address;
}

void PERSIST_SetPartPitch(uint8_t partPitch) {
	if (partPitch < 1) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			partPitch = 1;
		}
	PERSIST_partPitch = partPitch;
}

void PERSIST_SetFeedSpeed(uint8_t feedSpeed) {
	if (feedSpeed < 1) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		feedSpeed = 1;
	}
	PERSIST_feedSpeed = feedSpeed;
}

void PERSIST_SetDisplayBrightness(uint8_t displayBrightness) {
	if (displayBrightness < 1) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			displayBrightness = 1;
		}
	PERSIST_displayBrightness = displayBrightness;
}

void PERSIST_SetMotorDir(uint8_t motorDir) {
	if (motorDir > 1) {
		motorDir = 1;
		ERROR_SetError(ERROR_INVALID_INPUT);
	}
	PERSIST_motorDir = motorDir;
}

void PERSIST_SetMotorSlowDelay(uint16_t motorSlowDelay) {
	PERSIST_motorSlowDelay = motorSlowDelay;
}

void PERSIST_SetTotalParts(int32_t totalParts) {
	PERSIST_totalParts = totalParts;
}

void PERSIST_SetRemainingParts(int32_t remainingParts) {
	PERSIST_remainingParts = remainingParts;
}

void PERSIST_SetLowPartWarn(uint16_t lowPartWarn) {
	PERSIST_lowPartWarn = lowPartWarn;
}

void PERSIST_SetShortPartID(char* shortPartID, uint8_t length) {
	if (length < 1) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		return;
	}
	if (length > 6) {
		ERROR_SetError(ERROR_INVALID_INPUT);
		return;
	}
	if (length == 6) {
		if (shortPartID[5] != 0x00) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			return;
		}
	}
	int i = 0;
	while (i < length) {
		PERSIST_shortPartID[i] = shortPartID[i];
		i++;
	}
	while (i < 6) {
		PERSIST_shortPartID[i] = 0x00;
		i++;
	}
}

void PERSIST_SetLongPartID(char* longPartID, uint8_t length) {
	if (length < 1) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			return;
		}
		if (length > 127) {
			ERROR_SetError(ERROR_INVALID_INPUT);
			return;
		}
		if (length == 127) {
			if (longPartID[127] != 0x00) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				return;
			}
		}
		int i = 0;
		while (i < length) {
			PERSIST_longPartID[i] = longPartID[i];
			i++;
		}
		while (i < 6) {
			PERSIST_longPartID[i] = 0x00;
			i++;
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
}
