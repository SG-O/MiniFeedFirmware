/*
 * backup.c
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
	if (address > 4) address = 0;
	HAL_RTCEx_BKUPWrite(BACKUP_hrtc, address, data);
}

uint32_t BACKUP_Read(uint8_t address) {
	if (address > 4) address = 0;
	return HAL_RTCEx_BKUPRead(BACKUP_hrtc, address);
}
