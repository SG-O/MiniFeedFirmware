/*
 * ui.h
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
