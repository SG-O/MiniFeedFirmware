/*
 * hardware.h
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
