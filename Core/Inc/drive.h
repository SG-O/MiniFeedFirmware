/*
 * drive.h
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
 *  Created on: Jan. 18, 2021
 *      Author: SG-O
 */

#ifndef DRIVE_H_
#define DRIVE_H_

#include "main.h"

#define DRV_DIR_FOREWARD 1
#define DRV_DIR_REVERSE 1

TIM_HandleTypeDef *DRV_htim;
TIM_HandleTypeDef *DRV_delay;

void DRV_Start(TIM_HandleTypeDef *motor, TIM_HandleTypeDef *delay);
void DRV_Wake(void);
void DRV_Sleep(void);
void DRV_Spin(uint8_t speed, uint8_t direction);
void DRV_Feed(uint8_t uiCaused);
void DRV_Allign(void);
void DRV_ProcessSensorV0(void);
void DRV_ProcessSensorV1(void);
void DRV_ProcessTimer(void);

#endif /* DRIVE_H_ */
