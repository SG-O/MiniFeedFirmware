/*
 * menu.c
 *
 *  Created on: Jan. 29, 2021
 *      Author: SG-O
 */

#include "menu.h"

void MENU_Setup(u8g2_t *u8g2) {
	MENU_u8g2 = u8g2;
}

void MENU_DrawPage(uint8_t page) {
	char snum[16];
	u8g2_SetFont(MENU_u8g2, u8g2_font_t0_11b_mr);
	switch (page) {
	case 0:
		u8g2_DrawStr(MENU_u8g2, 2, 25, __STRG_BRIGHTNESS);
		uint8_t bright = PERSIST_GetDisplayBrightness();
		uint8_t i;
		for (i = 0; i < 5; i++) {
			if (bright < 2) {
				snum[i] = '|';
				i++;
				break;
			}
			snum[i] = '-';
			bright = bright >> 2;
		}
		while (i < 5) {
			snum[i] = '-';
			i++;
		}
		snum[i] = 0;
		u8g2_DrawStr(MENU_u8g2, 5, 36, snum);
		u8g2_DrawHLine(MENU_u8g2, 0, 41, 40);
		u8g2_DrawStr(MENU_u8g2, 2, 55, __STRG_PART_PITCH);
		sprintf(snum, "%umm", PERSIST_GetPartPitch());
		u8g2_DrawStr(MENU_u8g2, 2, 66, snum);
		break;
	case 1:
		u8g2_DrawStr(MENU_u8g2, 2, 25, __STRG_I2C_ADDRESS);
		if (PERSIST_GetI2CAddress() > 0x4F) {
			uint8_t address = COM_GetAddress();
			if (address > 15) {
				sprintf(snum, "A: %d", COM_GetAddress() - 16);
			} else {
				sprintf(snum, "A: ?");
			}
		} else {
			sprintf(snum, "%u", PERSIST_GetI2CAddress() - 0x10);
		}
		u8g2_DrawStr(MENU_u8g2, 2, 36, snum);
		u8g2_DrawHLine(MENU_u8g2, 0, 41, 40);
		u8g2_DrawStr(MENU_u8g2, 2, 55, __STRG_RESET);
		break;
	case 2:
		u8g2_DrawStr(MENU_u8g2, 2, 25, __STRG_DEBUG);
		u8g2_DrawHLine(MENU_u8g2, 0, 41, 40);
		u8g2_DrawStr(MENU_u8g2, 2, 55, __STRG_EXIT);
		break;
	}
}

void MENU_ChangeEntry(uint8_t entry, uint8_t direction) {
	switch (entry) {
		case MENU_ENTRY_BRIGHTNESS:
			if (direction == 1) {
				if (PERSIST_GetDisplayBrightness() > 63) {
					DISP_SetBrightness(255);
					PERSIST_SetDisplayBrightness(255);
				} else {
					uint8_t bright = PERSIST_GetDisplayBrightness() << 2;
					DISP_SetBrightness(bright);
					PERSIST_SetDisplayBrightness(bright);
				}
			} else {
				if (PERSIST_GetDisplayBrightness() < 5) {
					DISP_SetBrightness(1);
					PERSIST_SetDisplayBrightness(1);
				} else {
					uint8_t bright = PERSIST_GetDisplayBrightness() >> 2;
					DISP_SetBrightness(bright);
					PERSIST_SetDisplayBrightness(bright);
				}
			}
			break;
		case MENU_ENTRY_PITCH:
			if (direction == 1) {
				if (PERSIST_GetPartPitch() > 247) {
					PERSIST_SetPartPitch(252);
				} else {
					uint8_t pitch = PERSIST_GetPartPitch();
					if (pitch < 2) {
						pitch += 1;
					} else if (pitch < 4) {
						pitch += 2;
						pitch -= pitch % 2;
					} else {
						pitch += 4;
						pitch -= pitch % 4;
					}
					PERSIST_SetPartPitch(pitch);
				}
			} else {
				if (PERSIST_GetPartPitch() < 3) {
					PERSIST_SetPartPitch(1);
				} else if (PERSIST_GetPartPitch() < 7) {
					PERSIST_SetPartPitch(2);
				} else {
					uint8_t pitch = PERSIST_GetPartPitch() - 4;
					pitch -= pitch % 4;
					PERSIST_SetPartPitch(pitch);
				}
			}
			break;
		case MENU_ENTRY_ADDRESS:
			if (direction == 1) {
				if (PERSIST_GetI2CAddress() > 0x4F) {
					PERSIST_SetI2CAddress(0x10);
					COM_SetAddress();
				} else {
					uint8_t address = PERSIST_GetI2CAddress() + 1;
					PERSIST_SetI2CAddress(address);
					COM_SetAddress();
				}
			} else {
				if (PERSIST_GetI2CAddress() < 0x11) {
					PERSIST_SetI2CAddress(0x50);
					COM_SetAddress();
				} else {
					uint8_t address = PERSIST_GetI2CAddress() - 1;
					PERSIST_SetI2CAddress(address);
					COM_SetAddress();
				}
			}
			break;
		case MENU_ENTRY_RESET:
			NVIC_SystemReset();
			break;
		case MENU_ENTRY_DEBUG:
			UI_GoDebug();
			break;
		case MENU_ENTRY_EXIT:
			UI_GoHome();
			break;
	}
}
