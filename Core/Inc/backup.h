/*
 * backup.h
 *
 *  Created on: Jan 31, 2021
 *      Author: SG-O
 */

#ifndef INC_BACKUP_H_
#define INC_BACKUP_H_

#include "stm32g0xx_hal.h"

#define BACKUP_REG_MESSAGE 0

#define BACKUP_MESSAGE_BOOTLOADER 1 << 0
#define BACKUP_MESSAGE_INIT_EEPROM 1 << 1

RTC_HandleTypeDef *BACKUP_hrtc;

void BACKUP_Setup(RTC_HandleTypeDef *hrtc);
void BACKUP_Write(uint8_t address, uint32_t data);
uint32_t BACKUP_Read(uint8_t address);

#endif /* INC_BACKUP_H_ */
