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

uint8_t HW_version;
uint8_t HW_beta;

void HW_Setup(void);
uint64_t HW_GetFirmwareVersion(void);
uint64_t HW_GetHardwareVersion(void);
void HW_StartBootloader(void);

#endif /* HARDWARE_H_ */
