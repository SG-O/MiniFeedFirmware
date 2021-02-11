/*
 * hardware.c
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
	}
	HW_version = 0;
	if (!HAL_GPIO_ReadPin(HW_V1_GPIO_Port, HW_V1_Pin)){
		HW_version += 1;
	}
	if (!HAL_GPIO_ReadPin(HW_V1_GPIO_Port, HW_V1_Pin)){
		HW_version += 2;
	}
}

uint64_t HW_GetFirmwareVersion() {
	uint64_t value = VERSION_MAJOR & 0xFFFF;
	value = value << 16;
	value |= VERSION_MINOR & 0xFFFF;
	value = value << 32;
	value |= VERSION_PATCH & 0xFFFFFFFF;
	return value;
}

uint64_t HW_GetHardwareVersion() {
	uint64_t value = HW_beta & 0xFF;
	value = value << 8;
	value |= HW_version & 0xFF;
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
	SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));
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
