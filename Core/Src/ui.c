/*
 * ui.c
 *
 *  Created on: Jan. 29, 2021
 *      Author: SG-O
 */

#include "ui.h"

uint32_t start;

uint8_t oldButtonA = 0;
uint8_t oldButtonB = 0;
uint8_t oldButtonC = 0;

uint8_t blockB = 0;

uint32_t holdB;

uint8_t mode = UI_MODE_HOME;

uint8_t menuEntry = 0;

void UI_ShowBoot() {
	start = HAL_GetTick();
	DISP_DrawBoot();
}

void UI_GoHome() {
	mode = UI_MODE_HOME;
}

void UI_GoDebug() {
	mode = UI_MODE_DEBUG;
}

void UI_Process() {
	if (HAL_GetTick() - start < 2000) {
		return;
	}
	if ((ERROR_GetError() != ERROR_NO_ERROR) && (mode == UI_MODE_HOME)){
		mode = UI_MODE_ERROR;
	}
	if (mode == UI_MODE_ERROR) {
		if (ERROR_GetError() == ERROR_NO_ERROR){
			mode = UI_MODE_HOME;
		} else {
			DISP_DrawError();
		}
	}
	if (mode == UI_MODE_HOME) {
		DISP_DrawInfo();
	}
	if (mode == UI_MODE_DEBUG) {
		DISP_DrawDebug();
		ANALOG_StartConversion();
	}
	if (mode == UI_MODE_MENU) {
		DISP_DrawMenu(menuEntry);
	}

	if (!HAL_GPIO_ReadPin(Button_B_GPIO_Port, Button_B_Pin)) {
		if (oldButtonB == 0) {
			holdB = HAL_GetTick() + 2000;
		} else if (HAL_GetTick() > holdB) {
			blockB = 1;
			holdB = HAL_GetTick() + 2000;
			if (mode == UI_MODE_HOME) {
				menuEntry = 0;
				mode = UI_MODE_MENU;
			} else if (mode == UI_MODE_MENU) {
				mode = UI_MODE_HOME;
			} else if (mode == UI_MODE_DEBUG) {
				mode = UI_MODE_HOME;
			}
		}
		oldButtonB = 1;
	} else {
		if (blockB == 1) {
			blockB = 0;
			oldButtonB = 0;
		}
		if (oldButtonB == 1) {
			oldButtonB = 0;
			if ((mode == UI_MODE_HOME) || (mode == UI_MODE_HOME)) {
				DRV_Feed(1);
			} else if (mode == UI_MODE_MENU) {
				menuEntry++;
				if (menuEntry > MENU_ENTRY_EXIT) {
					menuEntry = 0;
				}
			} else if (mode == UI_MODE_ERROR) {
				ERROR_ResetError();
			}
		}
	}
	if (!HAL_GPIO_ReadPin(Button_A_GPIO_Port, Button_A_Pin)) {
		oldButtonA = 1;
	} else {
		if (oldButtonA == 1) {
			oldButtonA = 0;
			if (mode == UI_MODE_HOME) {
				DRV_Spin(255, 0);
			} else if (mode == UI_MODE_MENU) {
				MENU_ChangeEntry(menuEntry, 0);
			}

		}
	}
	if (!HAL_GPIO_ReadPin(Button_C_GPIO_Port, Button_C_Pin)) {
		oldButtonC = 1;
	} else {
		if (oldButtonC == 1) {
			oldButtonC = 0;
			if (mode == UI_MODE_HOME) {
				DRV_Spin(127, 1);
			} else if (mode == UI_MODE_MENU) {
				MENU_ChangeEntry(menuEntry, 1);
			}
		}
	}
}
