/*
 * drive.h
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
void DRV_ProcessSensorV0(void);
void DRV_ProcessSensorV1(void);
void DRV_ProcessTimer(void);

#endif /* DRIVE_H_ */
