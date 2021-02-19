/*
 * menu.h
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
 *  Created on: Jan. 29, 2021
 *      Author: SG-O
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "u8g2.h"
#include "persist.h"
#include "ui.h"
#include "com.h"

#define MENU_ENTRY_BRIGHTNESS 0
#define MENU_ENTRY_PITCH 1
#define MENU_ENTRY_ADDRESS 2
#define MENU_ENTRY_RESET 3
#define MENU_ENTRY_DEBUG 4
#define MENU_ENTRY_EXIT 5

u8g2_t *MENU_u8g2;

void MENU_Setup(u8g2_t *u8g2);
void MENU_DrawPage(uint8_t page);
void MENU_ChangeEntry(uint8_t entry, uint8_t direction);

#endif /* INC_MENU_H_ */
