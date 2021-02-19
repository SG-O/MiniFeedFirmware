/*
 * backup.c
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

#include "backup.h"

void BACKUP_Setup(RTC_HandleTypeDef *hrtc) {
	BACKUP_hrtc = hrtc;
	HAL_RTCEx_DeactivateInternalTamper(hrtc, RTC_INT_TAMPER_ALL);
}

void BACKUP_Write(uint8_t address, uint32_t data) {
	if (address > BACKUP_ADDRESSES) address = 0;
	HAL_RTCEx_BKUPWrite(BACKUP_hrtc, address, data);
}

uint32_t BACKUP_Read(uint8_t address) {
	if (address > BACKUP_ADDRESSES) address = 0;
	return HAL_RTCEx_BKUPRead(BACKUP_hrtc, address);
}
