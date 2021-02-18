/*
 * drive.c
 *
 *  Created on: Jan. 18, 2021
 *      Author: SG-O
 */
#include "drive.h"
#include "persist.h"

uint8_t DRV_count;
uint8_t DRV_speed;

uint8_t DRV_pol;

void DRV_Start(TIM_HandleTypeDef *motor, TIM_HandleTypeDef *delay) {
	DRV_htim = motor;
	DRV_delay = delay;
	DRV_Spin(0,0);
	DRV_Wake();
}

void DRV_Wake() {
	HAL_GPIO_WritePin(M_Sleep_GPIO_Port, M_Sleep_Pin, 1);
}

void DRV_Sleep() {
	HAL_GPIO_WritePin(M_Sleep_GPIO_Port, M_Sleep_Pin, 0);
}

void DRV_Spin(uint8_t speed, uint8_t direction) {
	direction = direction ^ PERSIST_GetMotorDir();
	HAL_TIM_PWM_Stop(DRV_htim, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(M_PH_GPIO_Port, M_PH_Pin, direction);
	TIM_OC_InitTypeDef sConfigOC;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = speed;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(DRV_htim, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(DRV_htim, TIM_CHANNEL_1);
}

void DRV_Feed(uint8_t uiCaused) {
	uint8_t count = 0;
	if (HW_IsV1() == 1) {
		count = PERSIST_GetPartPitch() >> 1;
	} else {
		count = PERSIST_GetPartPitch() >> 2;
	}
	uint8_t speed = PERSIST_GetFeedSpeed();
	if (speed < 1) {
		ERROR_SetError(ERROR_CONFIG_ERROR);
		return;
	}
	if (count < 1) {
		ERROR_SetError(ERROR_CONFIG_ERROR);
		return;
	}
	if (DRV_count > 1) {
		if (uiCaused == 0){
			ERROR_SetError(ERROR_BUSY);
		}
		return;
	}
	if (STATUS_GetStatus() != STATUS_READY) {
		if (uiCaused == 0){
			ERROR_SetError(ERROR_BUSY);
		}
		return;
	}
	STATUS_SetBusy();
	DRV_count = count;
	DRV_Spin(speed, 1);
	DRV_speed = speed;
	if (HW_IsV1() == 1) {
		GPIO_PinState sA = HAL_GPIO_ReadPin(V1_Sensor_A_GPIO_Port, V1_Sensor_A_Pin);
		GPIO_PinState sD = HAL_GPIO_ReadPin(V1_Sensor_D_GPIO_Port, V1_Sensor_D_Pin);
		if (!sA && !sD) {
			DRV_pol = 0;
		} else if (!sA && sD) {
			DRV_pol = 1;
		} else if (sA && sD) {
			DRV_pol = 2;
		} else {
			DRV_pol = 3;
		}
	} else {
		if (HAL_GPIO_ReadPin(Sensor_A_GPIO_Port, Sensor_A_Pin)) {
			DRV_pol = 1;
		} else {
			DRV_pol = 0;
		}
	}
	PERSIST_DecRemainingParts();
	if (speed < 128) return;
	if (DRV_count > 1) return;
	uint16_t slowdown = PERSIST_GetMotorSlowDelay();
	if (slowdown < 1) return;
	HAL_TIM_Base_Stop_IT(DRV_delay);
	int i = 0;
	while (HAL_TIM_Base_GetState(DRV_delay) != HAL_TIM_STATE_READY) {
		HAL_Delay(1);
		i++;
		if(i > 20) {
			DRV_speed = DRV_speed >> 1;
			DRV_Spin(DRV_speed, 1);
			return;
		}
	}
	DRV_delay->Instance->ARR = (uint32_t)slowdown;
	HAL_TIM_Base_Start_IT(DRV_delay);
}

void DRV_ProcessSensorV0(){
	if (DRV_count < 1) {
		if (STATUS_GetStatus() != STATUS_BUSY) return;

	}
	uint8_t update = 0;
	if (HAL_GPIO_ReadPin(Sensor_A_GPIO_Port, Sensor_A_Pin) && !DRV_pol) {
		if (DRV_count > 0){
			DRV_count --;
			update = 1;
		}
		DRV_pol = 1;
	}
	if (!HAL_GPIO_ReadPin(Sensor_A_GPIO_Port, Sensor_A_Pin) && HAL_GPIO_ReadPin(Sensor_B_GPIO_Port, Sensor_B_Pin) && DRV_pol) {
		if (DRV_count > 0){
			DRV_count--;
			update = 1;
		}
		DRV_pol = 0;
	}
	uint16_t slowdown = PERSIST_GetMotorSlowDelay();
	if ((DRV_count == 1) && (update == 1) && (slowdown > 0)) {
		HAL_TIM_Base_Stop_IT(DRV_delay);
		int i = 0;
		while (HAL_TIM_Base_GetState(DRV_delay) != HAL_TIM_STATE_READY) {
			HAL_Delay(1);
			i++;
			if(i > 20) {
				DRV_speed = DRV_speed >> 1;
				DRV_Spin(DRV_speed, 1);
				return;
			}
		}
		DRV_delay->Instance->ARR = (uint32_t)slowdown;
		HAL_TIM_Base_Start_IT(DRV_delay);
	}
	if (DRV_count < 1) {
		DRV_Spin(0, 0);
		HAL_TIM_Base_Stop_IT(DRV_delay);
		STATUS_SetReady();
	}
}

void DRV_ProcessSensorV1(){
	if (DRV_count < 1) {
		if (STATUS_GetStatus() != STATUS_BUSY) return;

	}
	uint8_t update = 0;
	GPIO_PinState sA = HAL_GPIO_ReadPin(V1_Sensor_A_GPIO_Port, V1_Sensor_A_Pin);
	GPIO_PinState sB = HAL_GPIO_ReadPin(V1_Sensor_B_GPIO_Port, V1_Sensor_B_Pin);
	GPIO_PinState sC = HAL_GPIO_ReadPin(V1_Sensor_C_GPIO_Port, V1_Sensor_C_Pin);
	GPIO_PinState sD = HAL_GPIO_ReadPin(V1_Sensor_D_GPIO_Port, V1_Sensor_D_Pin);
	if (!sA && sB && !sC && sD && DRV_pol == 0) {
		if (DRV_count > 0){
			DRV_count --;
			update = 1;
		}
		DRV_pol = 1;
	}
	if (sA && !sB && !sC && sD && DRV_pol == 1) {
		if (DRV_count > 0){
			DRV_count--;
			update = 1;
		}
		DRV_pol = 2;
	}
	if (sA && !sB && sC && !sD && DRV_pol == 2) {
		if (DRV_count > 0){
			DRV_count--;
			update = 1;
		}
		DRV_pol = 3;
	}
	if (!sA && sB && sC && !sD && DRV_pol == 3) {
		if (DRV_count > 0){
			DRV_count--;
			update = 1;
		}
		DRV_pol = 0;
	}
	uint16_t slowdown = PERSIST_GetMotorSlowDelay();
	if ((DRV_count == 1) && (update == 1) && (slowdown > 0)) {
		HAL_TIM_Base_Stop_IT(DRV_delay);
		int i = 0;
		while (HAL_TIM_Base_GetState(DRV_delay) != HAL_TIM_STATE_READY) {
			HAL_Delay(1);
			i++;
			if(i > 20) {
				DRV_speed = DRV_speed >> 1;
				DRV_Spin(DRV_speed, 1);
				return;
			}
		}
		DRV_delay->Instance->ARR = (uint32_t)slowdown;
		HAL_TIM_Base_Start_IT(DRV_delay);
	}
	if (DRV_count < 1) {
		DRV_Spin(0, 0);
		HAL_TIM_Base_Stop_IT(DRV_delay);
		STATUS_SetReady();
	}
}

void DRV_ProcessTimer(){
	if (DRV_count < 1) return;
	if (DRV_speed < 96) return;
	if (STATUS_GetStatus() != STATUS_BUSY) return;
	DRV_speed = DRV_speed >> 1;
	DRV_Spin(DRV_speed, 1);
	HAL_TIM_Base_Stop_IT(DRV_delay);
	int i = 0;
	while (HAL_TIM_Base_GetState(DRV_delay) != HAL_TIM_STATE_READY) {
		HAL_Delay(1);
		i++;
		if(i > 20) return;
	}
	HAL_TIM_Base_Start_IT(DRV_delay);
}
