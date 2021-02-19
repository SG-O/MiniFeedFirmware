/*
 * display.h
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
 *      Author: User
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "main.h"
#include "u8g2.h"
#include "persist.h"
#include "menu.h"
#include "analog.h"

#define I2C_ADDRESS 0x3C

#define DISP_LOGO_WIDTH 40
#define DISP_LOGO_HEIGHT 25

#define DISP_TX_BUFFER_LENGTH 32
#define DISP_TX_TIMEOUT 10
#define DISP_STRING_BUFFER_LENGTH 16

#define DISP_DEBUG_PAGES 2

u8g2_t u8g2;
I2C_HandleTypeDef *DISP_hi2c;

void DISP_Setup(I2C_HandleTypeDef *hi2c);
void DISP_DrawDebug(uint8_t page);
void DISP_DrawInfo(void);
void DISP_DrawError(void);
void DISP_DrawBoot(void);
void DISP_DrawBootloader(void);
void DISP_DrawMenu(uint8_t entry);
void DISP_SetBrightness(uint8_t bright);

#endif /* DISPLAY_H_ */
