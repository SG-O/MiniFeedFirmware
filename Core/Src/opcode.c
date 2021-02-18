/*
 * opcode.c
 *
 *  Created on: Jan. 26, 2021
 *      Author: SG-O
 */

#include "opcode.h"


uint8_t OPCODE_ParseLong(uint8_t opcode, uint8_t length, uint8_t *data){
	switch(opcode) {
		case OPCODE_REMAINING_PARTS:
			if (length < 4) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetRemainingParts(CON_BytesToSigned32(data, 4));
			break;
		case OPCODE_TOTAL_PARTS:
			if (length < 4) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetTotalParts(CON_BytesToSigned32(data, 4));
			break;
		case OPCODE_PART_PITCH:
			if (length < 1) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetPartPitch(data[0]);
			break;
		case OPCODE_FEED_SPEED:
			if (length < 1) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetFeedSpeed(data[0]);
			break;
		case OPCODE_BRIGHTNESS:
			if (length < 1) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetDisplayBrightness(data[0]);
			DISP_SetBrightness(PERSIST_GetDisplayBrightness());
			break;
		case OPCODE_MOT_DIR:
			if (length < 1) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetMotorDir(data[0]);
			break;
		case OPCODE_LOW_PARTS:
			if (length < 2) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetLowPartWarn(CON_BytesToUnsigned16(data, 2));
			break;
		case OPCODE_MOT_SLOWDOWN:
			if (length < 2) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetMotorSlowDelay(CON_BytesToUnsigned16(data, 2));
			break;
		case OPCODE_SHORT_ID:
			if (length < 1) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetShortPartID((char*)data, length);
			break;
		case OPCODE_LONG_ID:
			if (length < 1) {
				ERROR_SetError(ERROR_INVALID_INPUT);
				break;
			}
			PERSIST_SetLongPartID((char*)data, length);
			break;
		default:
			ERROR_SetError(ERROR_UNKNOWN_METHOD);
			break;
	}
	return ERROR_GetError();
}

uint8_t OPCODE_ParseShort(uint8_t opcode){
	switch(opcode) {
		case OPCODE_NOP:
			break;
		case OPCODE_FEED:
			DRV_Feed(0);
			break;
		case OPCODE_RESET_ERROR:
			ERROR_ResetError();
			break;
		case OPCODE_FORCE_ERROR:
			ERROR_SetError(ERROR_USER_CAUSED);
			break;
		case OPCODE_INIT_EEPROM:
			if (HW_IsV1() == 1) {
				EEPROM_WriteHeader();
				EEPROM_WriteDefaults();
			} else {
				ERROR_SetError(ERROR_UNKNOWN_METHOD);
			}
			break;
		case OPCODE_ENTER_BOOTLOADER:
			HW_StartBootloader();
			break;
		case OPCODE_RESET:
			NVIC_SystemReset();
			break;
		default:
			ERROR_SetError(ERROR_UNKNOWN_METHOD);
			break;
	}
	return ERROR_GetError();
}

uint8_t OPCODE_Parse(uint8_t opcode, uint8_t length, uint8_t *data) {
	if (opcode > 127) {
		return OPCODE_ParseShort(opcode);
	} else {
		return OPCODE_ParseLong(opcode, length, data);
	}
}
