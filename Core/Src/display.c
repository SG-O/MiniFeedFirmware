/*
 * display.c
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
 *  Created on: Jan. 17, 2021
 *      Author: SG-O
 */

#include "display.h"
uint32_t lastBlink = 0;

static const uint8_t DISP_LOGO[] = {
  0x87, 0x07, 0xE3, 0xFF, 0xE3, 0x87, 0x07, 0xE3, 0xFF, 0xE3, 0x87, 0x07,
  0xFF, 0xFF, 0xFF, 0x07, 0x03, 0x63, 0x84, 0xE3, 0x07, 0x03, 0x63, 0x00,
  0xE3, 0x07, 0x02, 0x63, 0x00, 0xE3, 0x47, 0x08, 0x63, 0x1C, 0xE3, 0x47,
  0x08, 0x63, 0x1C, 0xE3, 0x47, 0x08, 0x63, 0x1C, 0xE3, 0xC7, 0x0C, 0x63,
  0x1C, 0xE3, 0xC7, 0x0C, 0x63, 0x1C, 0xE3, 0xC7, 0x0C, 0x63, 0x1C, 0xE3,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFE,
  0xFF, 0xFF, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x1F, 0xF0, 0x1F, 0x7F, 0xF8,
  0x11, 0xF0, 0x07, 0x3C, 0xE0, 0x00, 0x00, 0x43, 0x18, 0x63, 0x00, 0x00,
  0xE3, 0x08, 0x47, 0x18, 0x00, 0x03, 0x08, 0x40, 0x1C, 0xF0, 0x03, 0x08,
  0x40, 0x18, 0xF0, 0xE3, 0x1F, 0x7F, 0x18, 0xF0, 0x03, 0x1C, 0xE0, 0x00,
  0xF0, 0x07, 0x3E, 0xF0, 0x00, };

uint8_t DISP_CalculateStringOffset(const char *strg, int16_t charWidth) {
	int16_t length = strlen(strg);
	int16_t width = u8g2_GetDisplayWidth(&u8g2);
	int16_t totalLength = length * charWidth;
	int16_t space = width - totalLength;
	if (space < 1) return 0;
	return (uint8_t)(space>>1);
}

uint8_t DISP_U8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_GPIO_AND_DELAY_INIT:
	  HAL_Delay(1);
	  break;
  case U8X8_MSG_DELAY_MILLI:
	  HAL_Delay(arg_int);
	  break;
  case U8X8_MSG_GPIO_RESET:
	  HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, arg_int);
	  break;
  default:
	  break;
  }
  return 1;
}

uint8_t DISP_U8x8_byte_stm32hal_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	static uint8_t buffer[DISP_TX_BUFFER_LENGTH];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
	static uint8_t buf_idx;
	uint8_t *data;


  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
    {
        data = (uint8_t *)arg_ptr;
        while( arg_int > 0 )
        {
			buffer[buf_idx++] = *data;
			data++;
			arg_int--;
        }
    }
    	break;
    case U8X8_MSG_BYTE_INIT:
    	break;
    case U8X8_MSG_BYTE_SET_DC:
    	break;
    case U8X8_MSG_BYTE_START_TRANSFER:
		{
			buf_idx = 0;
		}
		break;
    case U8X8_MSG_BYTE_END_TRANSFER:
		while (HAL_I2C_GetState(DISP_hi2c) != HAL_I2C_STATE_READY)
		{
			asm("NOP");
		}
		HAL_I2C_Master_Transmit(DISP_hi2c, (uint16_t)I2C_ADDRESS<<1, &buffer[0], buf_idx, DISP_TX_TIMEOUT);
    	break;
    default:
      return 0;
  }
  return 1;
}

void DISP_Setup(I2C_HandleTypeDef *hi2c){
	DISP_hi2c = hi2c;
	if (HW_IsV1() == 0) {
		u8g2_Setup_ssd1306_i2c_72x40_er_f(&u8g2, U8G2_R1, DISP_U8x8_byte_stm32hal_hw_i2c, DISP_U8x8_stm32_gpio_and_delay);
	} else {
		u8g2_Setup_ssd1306_i2c_72x40_er_f(&u8g2, U8G2_R3, DISP_U8x8_byte_stm32hal_hw_i2c, DISP_U8x8_stm32_gpio_and_delay);
	}
	u8g2_SetI2CAddress(&u8g2, I2C_ADDRESS*2);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_ClearDisplay(&u8g2);
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_ClearBuffer(&u8g2);
	u8g2_SendBuffer(&u8g2);
	MENU_Setup(&u8g2);
}

void DISP_SetBrightness(uint8_t bright){
	if (bright < 1) return;
	u8g2_SetContrast(&u8g2, bright);
}

void DISP_DrawInfo(){
	char snum[DISP_STRING_BUFFER_LENGTH];

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFontPosBaseline(&u8g2);
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_DrawBox(&u8g2, 0, 0, 40, 12);
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_7x14B_mr);
	char *id = PERSIST_GetShortPartID();
	sprintf(snum, "%.5s", id);
	uint8_t offset = DISP_CalculateStringOffset(snum, 7);
	u8g2_DrawStr(&u8g2, offset, 11, snum);
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_SetFontPosCenter(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_10x20_mn);
	int32_t feedCount = PERSIST_GetRemainingParts();
	uint16_t width = 10;
	uint16_t yPos = 37;
	if ((feedCount > 999999) || (feedCount < -99999)) {
		feedCount = 8;
		u8g2_SetFontPosBaseline(&u8g2);
		u8g2_SetFontDirection(&u8g2, 1);
		u8g2_SetFont(&u8g2, u8g2_font_inr24_mn);
		width = 26;
		yPos = 27;
	} else if ((feedCount > 9999) || (feedCount < -999)) {
		u8g2_SetFont(&u8g2, u8g2_font_6x13_mn);
		width = 6;
	} else if ((feedCount < 100) && (feedCount > -10)) {
		u8g2_SetFont(&u8g2, u8g2_font_inr24_mn);
		width = 20;
		yPos = 40;
	}

	sprintf(snum, "%ld", feedCount);
	offset = DISP_CalculateStringOffset(snum, width);
	u8g2_DrawStr(&u8g2, offset, yPos, snum);

	u8g2_SetFontPosCenter(&u8g2);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_6x13_mn);
	u8g2_DrawHLine(&u8g2, 0, 60, 40);
	int32_t feedTotal = PERSIST_GetTotalParts();
	if ((feedTotal > 999999) || (feedTotal < 1)) {
		sprintf(snum, "-----");
	} else {
		sprintf(snum, "%ld", feedTotal);
	}
	offset = DISP_CalculateStringOffset(snum, 6);
	u8g2_DrawStr(&u8g2, offset, 68, snum);
	if (PERSIST_GetLowPartWarn() > 0) {
		if (feedCount < 1) {
			if (HAL_GetTick() > lastBlink + 500) {
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				lastBlink = HAL_GetTick();
			}
		} else {
			if (feedCount < PERSIST_GetLowPartWarn()) {
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
			} else {
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
			}
		}
	} else {
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
	}
	u8g2_SendBuffer(&u8g2);
}

void DISP_DrawBoot(){
	char snum[DISP_STRING_BUFFER_LENGTH];

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_SetFontPosBaseline(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11b_mr);
	u8g2_DrawXBM(&u8g2, 0, 0, DISP_LOGO_WIDTH, DISP_LOGO_HEIGHT, DISP_LOGO);
	u8g2_SetDrawColor(&u8g2, 1);

	sprintf(snum, "v%u", VERSION_MAJOR);
	u8g2_DrawStr(&u8g2, 1, 37, snum);
	sprintf(snum, ".%u", VERSION_MINOR);
	u8g2_DrawStr(&u8g2, 1, 47, snum);
	sprintf(snum, ".%u", VERSION_PATCH);
	u8g2_DrawStr(&u8g2, 1, 57, snum);
	u8g2_DrawStr(&u8g2, 1, 67, VERSION_DESCRIPTION);
	u8g2_SendBuffer(&u8g2);
}

void DISP_DrawError(){
	char snum[DISP_STRING_BUFFER_LENGTH];

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFontPosBaseline(&u8g2);
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_DrawBox(&u8g2, 0, 13, 40, 60);
	u8g2_SetFont(&u8g2, u8g2_font_7x14B_mr);
	uint8_t offset = DISP_CalculateStringOffset(__STRG_ERROR, 7);
	u8g2_DrawStr(&u8g2, offset, 11, __STRG_ERROR);
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_SetFontPosBaseline(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11b_mr);
	u8g2_SetFontPosCenter(&u8g2);

	sprintf(snum, "%u", ERROR_GetError());
	u8g2_SetFont(&u8g2, u8g2_font_inr24_mn);
	offset = DISP_CalculateStringOffset(snum, 20);
	u8g2_DrawStr(&u8g2, offset, 45, snum);
	if (HAL_GetTick() > lastBlink + 500) {
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		lastBlink = HAL_GetTick();
	}
	u8g2_SendBuffer(&u8g2);
}

void DISP_DrawBootloader(){
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFontPosBaseline(&u8g2);
	u8g2_DrawBox(&u8g2, 0, 0, 40, 72);
	u8g2_SetFont(&u8g2, u8g2_font_7x14B_mr);
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_DrawStr(&u8g2, 1, 31, __STRG_BOOTLOADER_L0);
	u8g2_DrawStr(&u8g2, 1, 42, __STRG_BOOTLOADER_L1);
	u8g2_SendBuffer(&u8g2);
}

void DISP_DrawDebug(uint8_t page){
	char snum[DISP_STRING_BUFFER_LENGTH];

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFontPosBaseline(&u8g2);
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_DrawBox(&u8g2, 0, 0, 40, 12);
	u8g2_DrawBox(&u8g2, 0, 62, 40, 10);
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_7x14B_mr);
	uint8_t offset = DISP_CalculateStringOffset(__STRG_DEBUG, 7);
	u8g2_DrawStr(&u8g2, offset, 11, __STRG_DEBUG);
	sprintf(snum, "P%u/%u", page + 1, DISP_DEBUG_PAGES);
	offset = DISP_CalculateStringOffset(snum, 6);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11b_mr);
	u8g2_DrawStr(&u8g2, offset, 71, snum);

	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_SetFontPosBaseline(&u8g2);
	switch (page) {
	case 0:
		if (HW_IsV1() == 1) {
			if (HAL_GPIO_ReadPin(V1_Sensor_A_GPIO_Port, V1_Sensor_A_Pin)) {
				u8g2_DrawStr(&u8g2, 1, 25, "S_A On");
			} else {
				u8g2_DrawStr(&u8g2, 1, 25, "S_A Off");
			}
			if (HAL_GPIO_ReadPin(V1_Sensor_B_GPIO_Port, V1_Sensor_B_Pin)) {
				u8g2_DrawStr(&u8g2, 1, 36, "S_B On");
			} else {
				u8g2_DrawStr(&u8g2, 1, 36, "S_B Off");
			}
			if (HAL_GPIO_ReadPin(V1_Sensor_C_GPIO_Port, V1_Sensor_C_Pin)) {
				u8g2_DrawStr(&u8g2, 1, 47, "S_C On");
			} else {
				u8g2_DrawStr(&u8g2, 1, 47, "S_C Off");
			}
			if (HAL_GPIO_ReadPin(V1_Sensor_D_GPIO_Port, V1_Sensor_D_Pin)) {
				u8g2_DrawStr(&u8g2, 1, 58, "S_D On");
			} else {
				u8g2_DrawStr(&u8g2, 1, 58, "S_D Off");
			}
		} else {
			if (HAL_GPIO_ReadPin(Sensor_A_GPIO_Port, Sensor_A_Pin)) {
				u8g2_DrawStr(&u8g2, 1, 25, "S_A On");
			} else {
				u8g2_DrawStr(&u8g2, 1, 25, "S_A Off");
			}
			if (HAL_GPIO_ReadPin(Sensor_B_GPIO_Port, Sensor_B_Pin)) {
				u8g2_DrawStr(&u8g2, 1, 36, "S_B On");
			} else {
				u8g2_DrawStr(&u8g2, 1, 36, "S_B Off");
			}
		}
		break;
	case 1:
		sprintf(snum, "%lumA", ANALOG_GetmaMotor());
		u8g2_DrawStr(&u8g2, 1, 25, snum);
		sprintf(snum, "%lumV", ANALOG_GetVRef());
		u8g2_DrawStr(&u8g2, 1, 36, snum);
		sprintf(snum, "%lumV", ANALOG_GetVID());
		u8g2_DrawStr(&u8g2, 1, 47, snum);
		break;
	default:
		break;
	}
	u8g2_SendBuffer(&u8g2);
}

void DISP_DrawMenu(uint8_t entry){
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFontPosBaseline(&u8g2);
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_DrawBox(&u8g2, 0, 0, 40, 12);
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_7x14B_mr);
	uint8_t offset = DISP_CalculateStringOffset(__STRG_MENU, 7);
	u8g2_DrawStr(&u8g2, offset, 11, __STRG_MENU);
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_SetFontPosBaseline(&u8g2);
	MENU_DrawPage(entry / 2);
	if ((entry % 2) == 0) {
		u8g2_DrawHLine(&u8g2, 0, 13, 40);
		u8g2_DrawVLine(&u8g2, 0, 13, 27);
		u8g2_DrawVLine(&u8g2, 39, 13, 27);
		u8g2_DrawHLine(&u8g2, 0, 39, 40);
	} else {
		u8g2_DrawHLine(&u8g2, 0, 43, 40);
		u8g2_DrawVLine(&u8g2, 0, 43, 27);
		u8g2_DrawVLine(&u8g2, 39, 43, 27);
		u8g2_DrawHLine(&u8g2, 0, 70, 40);
	}
	u8g2_SendBuffer(&u8g2);
}
