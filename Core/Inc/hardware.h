/*
 * hardware.h
 *
 *  Created on: Jan 17, 2021
 *      Author: SG-O
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "main.h"
#include "conversion.h"
#include "display.h"
#include "backup.h"

uint8_t HW_version;
uint8_t HW_beta;

void HW_Setup(void);
uint8_t HW_IsV1(void);
uint64_t HW_GetFirmwareVersion(void);
uint64_t HW_GetHardwareVersion(void);
void HW_StartBootloader(void);
void HW_CheckForBootloader(uint8_t resetWWDT);
void HW_ProcessBootloaderSchedule(void);

#endif /* HARDWARE_H_ */
