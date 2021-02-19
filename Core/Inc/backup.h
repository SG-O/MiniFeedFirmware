/*
 * backup.h
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
 *  Created on: Jan 31, 2021
 *      Author: SG-O
 */

#ifndef INC_BACKUP_H_
#define INC_BACKUP_H_

#include "stm32g0xx_hal.h"

#define BACKUP_ADDRESSES 4

#define BACKUP_REG_MESSAGE 0

#define BACKUP_MESSAGE_BOOTLOADER 1 << 0
#define BACKUP_MESSAGE_INIT_EEPROM 1 << 1

RTC_HandleTypeDef *BACKUP_hrtc;

void BACKUP_Setup(RTC_HandleTypeDef *hrtc);
void BACKUP_Write(uint8_t address, uint32_t data);
uint32_t BACKUP_Read(uint8_t address);

#endif /* INC_BACKUP_H_ */
