/*
 * value.c
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
		dataLength = CON_TRIPPLE_INT_LENGTH;
		COM_PutUnsigned8(dataLength);
		COM_PutUnsigned32(HAL_GetUIDw0());
		COM_PutUnsigned32(HAL_GetUIDw1());
		COM_PutUnsigned32(HAL_GetUIDw2());
		break;
	case VALUE_FIRMWARE_VERSION:
		dataLength = CON_LONG_LENGTH;
		COM_PutUnsigned8(dataLength);
		data64 = HW_GetFirmwareVersion();
		COM_PutUnsigned32((data64 >> CON_INT_SHIFT) & CON_INT_MASK);
		COM_PutUnsigned32(data64 & CON_INT_MASK);
		break;
	case VALUE_HARDWARE_VERSION:
		dataLength = CON_LONG_LENGTH;
		COM_PutUnsigned8(dataLength);
		data64 = HW_GetHardwareVersion();
		COM_PutUnsigned32((data64 >> CON_INT_SHIFT) & CON_INT_MASK);
		COM_PutUnsigned32(data64 & CON_INT_MASK);
		break;
	case VALUE_REMAINING_PARTS:
		dataLength = CON_INT_LENGTH;
		COM_PutUnsigned8(dataLength);
		COM_PutSigned32(PERSIST_GetRemainingParts());
		break;
	case VALUE_TOTAL_PARTS:
		dataLength = CON_INT_LENGTH;
		COM_PutUnsigned8(dataLength);
		COM_PutSigned32(PERSIST_GetTotalParts());
		break;
	case VALUE_LOW_PARTS:
		dataLength = CON_SHORT_LENGTH;
		COM_PutUnsigned8(dataLength);
		COM_PutUnsigned16(PERSIST_GetLowPartWarn());
		break;
	case VALUE_MOT_SLOWDOWN:
		dataLength = CON_SHORT_LENGTH;
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
			dataLength = CON_LONG_LENGTH;
			COM_PutUnsigned8(dataLength);
			data64 = PERSIST_GetTotalFeeds();
			COM_PutUnsigned32((data64 >> CON_INT_SHIFT) & CON_INT_MASK);
			COM_PutUnsigned32(data64 & CON_INT_MASK);
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
