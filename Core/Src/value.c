/*
 * value.c
 *
 *  Created on: Jan. 26, 2021
 *      Author: SG-O
 */

#include "value.h"

void VALUE_ParseLongRead(uint8_t value) {
	uint8_t i;
	uint64_t data64;
	char *strg;

	uint8_t dataLength = 0;
	switch(value) {
	case VALUE_FEEDER_ID:
		dataLength = 12;
		COM_PutUnsigned8(dataLength);
		COM_PutUnsigned32(HAL_GetUIDw0());
		COM_PutUnsigned32(HAL_GetUIDw1());
		COM_PutUnsigned32(HAL_GetUIDw2());
		break;
	case VALUE_FIRMWARE_VERSION:
		dataLength = 8;
		COM_PutUnsigned8(dataLength);
		data64 = HW_GetFirmwareVersion();
		COM_PutUnsigned32((data64 >> 32) & 0xFFFFFFFF);
		COM_PutUnsigned32(data64 & 0xFFFFFFFF);
		break;
	case VALUE_HARDWARE_VERSION:
		dataLength = 8;
		COM_PutUnsigned8(dataLength);
		data64 = HW_GetHardwareVersion();
		COM_PutUnsigned32((data64 >> 32) & 0xFFFFFFFF);
		COM_PutUnsigned32(data64 & 0xFFFFFFFF);
		break;
	case VALUE_REMAINING_PARTS:
		dataLength = 4;
		COM_PutUnsigned8(dataLength);
		COM_PutSigned32(PERSIST_GetRemainingParts());
		break;
	case VALUE_TOTAL_PARTS:
		dataLength = 4;
		COM_PutUnsigned8(dataLength);
		COM_PutSigned32(PERSIST_GetTotalParts());
		break;
	case VALUE_LOW_PARTS:
		dataLength = 2;
		COM_PutUnsigned8(dataLength);
		COM_PutUnsigned16(PERSIST_GetLowPartWarn());
		break;
	case VALUE_MOT_SLOWDOWN:
		dataLength = 2;
		COM_PutUnsigned8(dataLength);
		COM_PutUnsigned16(PERSIST_GetMotorSlowDelay());
		break;
	case VALUE_SHORT_ID:
		strg = PERSIST_GetShortPartID();
		dataLength = strlen(strg) + 1;
		COM_PutUnsigned8(dataLength);
		for (i = 0; i < dataLength; i++) {
			COM_PutUnsigned8((uint8_t)strg[i]);
		}
		break;
	case VALUE_LONG_ID:
		strg = PERSIST_GetLongPartID();
		dataLength = strlen(strg) + 1;
		COM_PutUnsigned8(dataLength);
		for (i = 0; i < dataLength; i++) {
			COM_PutUnsigned8((uint8_t)strg[i]);
		}
		break;
	case VALUE_TOTAL_FEEDS:
			dataLength = 8;
			COM_PutUnsigned8(dataLength);
			data64 = PERSIST_GetTotalFeeds();
			COM_PutUnsigned32((data64 >> 32) & 0xFFFFFFFF);
			COM_PutUnsigned32(data64 & 0xFFFFFFFF);
			break;
	default:
		COM_PutUnsigned8(0);
		return;
	}
	COM_GenerateLongCRC();
}

void VALUE_ParseShortRead(uint8_t value) {
	switch(value) {
	case VALUE_PROTO_VERSION:
		COM_PutUnsigned8(CONFIG_PROTO_VERSION);
		break;
	case VALUE_STATUS:
		COM_PutUnsigned8(STATUS_GetStatus());
		break;
	case VALUE_ERROR:
		COM_PutUnsigned8(ERROR_GetError());
		break;
	case VALUE_PART_PITCH:
		COM_PutUnsigned8(PERSIST_GetPartPitch());
		break;
	case VALUE_FEED_SPEED:
		COM_PutUnsigned8(PERSIST_GetFeedSpeed());
		break;
	case VALUE_BRIGHTNESS:
		COM_PutUnsigned8(PERSIST_GetDisplayBrightness());
		break;
	case VALUE_MOT_DIR:
		COM_PutUnsigned8(PERSIST_GetMotorDir());
		break;
	default:
		COM_PutUnsigned8(0);
		COM_PutUnsigned8(0);
		return;
	}
	COM_GenerateShortCRC();
}
