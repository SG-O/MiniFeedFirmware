/*
 * hardware.c
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

#include "hardware.h"


void HW_Setup() {
	if (HAL_GPIO_ReadPin(HW_MV_GPIO_Port, HW_MV_Pin)){
		HW_beta = 0;
	} else {
		HW_beta = 1;
		HAL_GPIO_DeInit(COM_SCL_GPIO_Port, COM_SCL_Pin);
		HAL_GPIO_DeInit(COM_SDA_GPIO_Port, COM_SDA_Pin);
		HAL_GPIO_DeInit(TX_GPIO_Port, TX_Pin|RX_Pin);

		GPIO_InitTypeDef GPIO_InitStruct0 = {0};
		GPIO_InitStruct0.Pin = V1_COM_SCL_Pin|V1_COM_SDA_Pin;
		GPIO_InitStruct0.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct0.Pull = GPIO_PULLUP;
		GPIO_InitStruct0.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct0.Alternate = GPIO_AF6_I2C1;
		HAL_GPIO_Init(V1_COM_SCL_GPIO_Port, &GPIO_InitStruct0);

		GPIO_InitTypeDef GPIO_InitStruct1 = {0};
		GPIO_InitStruct1.Pin = V1_TX_Pin|V1_RX_Pin;
		GPIO_InitStruct1.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct1.Pull = GPIO_NOPULL;
		GPIO_InitStruct1.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct1.Alternate = GPIO_AF0_USART1;
		HAL_GPIO_Init(V1_RX_GPIO_Port, &GPIO_InitStruct1);
	}
	HW_version = 0;
	if (!HAL_GPIO_ReadPin(HW_V1_GPIO_Port, HW_V1_Pin)){
		HW_version += 1;
	}
	if (!HAL_GPIO_ReadPin(HW_V1_GPIO_Port, HW_V1_Pin)){
		HW_version += 2;
	}
}

uint8_t HW_IsV1() {
	return HW_beta;
}

uint64_t HW_GetFirmwareVersion() {
	uint64_t value = VERSION_MAJOR & 0xFFFF;
	value = value << CON_SHORT_SHIFT;
	value |= VERSION_MINOR & 0xFFFF;
	value = value << CON_INT_SHIFT;
	value |= VERSION_PATCH & 0xFFFFFFFF;
	return value;
}

uint64_t HW_GetHardwareVersion() {
	uint64_t value = HW_beta & 0xFF;
	value = value << CON_BYTE_SHIFT;
	value |= HW_version & CON_BYTE_MASK;
	return value;
}

void HW_ProcessBootloaderSchedule() {
	uint32_t backupData = BACKUP_Read(BACKUP_REG_MESSAGE);
	if ((backupData && (BACKUP_MESSAGE_BOOTLOADER)) == 0) return;
	HAL_Delay(1000);
}

void HW_StartBootloader() {
	uint32_t backupData = BACKUP_Read(BACKUP_REG_MESSAGE);
	backupData |= BACKUP_MESSAGE_BOOTLOADER;
	BACKUP_Write(BACKUP_REG_MESSAGE, backupData);
}

void HW_InitBootloader() {
	void (*SysMemBootJump)(void);
	volatile uint32_t addr = 0x1FFF0000;
	SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4))); // @suppress("Avoid magic numbers")
	DISP_DrawBootloader();
	//Disable all peripheral clocks
	//Disable used PLL
	HAL_RCC_DeInit();
	HAL_DeInit();

	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL  = 0;

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

	__set_MSP(*(__IO uint32_t*)addr);
	//Disable interrupts
	__disable_irq();

	SysMemBootJump();
}

void HW_CheckForBootloader(uint8_t resetWWDT) {
	if (!resetWWDT) return;
	uint32_t backupData = BACKUP_Read(BACKUP_REG_MESSAGE);
	if ((backupData && (BACKUP_MESSAGE_BOOTLOADER)) == 0) return;
	backupData &= ~(BACKUP_MESSAGE_BOOTLOADER);
	BACKUP_Write(BACKUP_REG_MESSAGE, backupData);
	HW_InitBootloader();
}
