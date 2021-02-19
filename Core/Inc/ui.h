/*
 * ui.h
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

#ifndef INC_UI_H_
#define INC_UI_H_

#include "display.h"
#include "main.h"
#include "drive.h"
#include "menu.h"

#define UI_MODE_HOME 0
#define UI_MODE_MENU 1
#define UI_MODE_DEBUG 2
#define UI_MODE_ERROR 3

void UI_ShowBoot(void);
void UI_GoHome(void);
void UI_GoDebug(void);
void UI_Process(void);

#endif /* INC_UI_H_ */
