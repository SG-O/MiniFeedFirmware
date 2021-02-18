/*
 * eeprom.h
 *
 *  Created on: Feb. 15, 2021
 *      Author: SG-O
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "main.h"
#include "ascii85.h"
#include "backup.h"

#define EEPROM_FORMAT_VERSION 0x00

#define EEPROM_HEADER_OFFSET 0x0000
#define EEPROM_HEADER_FORMAT_VERSION_OFFSET 0x00
#define EEPROM_HEADER_DEVICE_ID_0_OFFSET 0x10
#define EEPROM_HEADER_DEVICE_ID_1_OFFSET 0x14
#define EEPROM_HEADER_DEVICE_ID_2_OFFSET 0x18
#define EEPROM_HEADER_CRC_OFFSET 0x1C
#define EEPROM_HEADER_LENGTH 0x20


#define EEPROM_CONFIG_0_OFFSET 0x0020

#define EEPROM_CONFIG_ROW_WRITE_0_OFFSET 0x00
#define EEPROM_CONFIG_PART_PITCH_OFFSET 0x04
#define EEPROM_CONFIG_FEED_SPEED_OFFSET 0x08
#define EEPROM_CONFIG_LOW_PARTS_WARNING_OFFSET 0x0C
#define EEPROM_CONFIG_ROW_WRITE_1_OFFSET 0x10
#define EEPROM_CONFIG_DISPLAY_BRIGHTNESS_OFFSET 0x14
#define EEPROM_CONFIG_MOTOR_DIRECTION_OFFSET 0x18
#define EEPROM_CONFIG_MOTOR_SLOWDOWN_DELAY_OFFSET 0x1C
#define EEPROM_CONFIG_ROW_WRITE_2_OFFSET 0x20
#define EEPROM_CONFIG_TOTAL_PARTS_OFFSET 0x24
#define EEPROM_CONFIG_ROW_WRITE_3_OFFSET 0x30
#define EEPROM_CONFIG_SHORT_ID_OFFSET 0x34
#define EEPROM_CONFIG_SHORT_ID_FILLER_0_OFFSET 0x3A
#define EEPROM_CONFIG_SHORT_ID_FILLER_1_OFFSET 0x3B
#define EEPROM_CONFIG_LONG_ID_WRITES_OFFSET 0x40
#define EEPROM_CONFIG_LONG_ID_OFFSET 0x50
#define EEPROM_CONFIG_LONG_ID_FILLER_OFFSET 0xCF

#define EEPROM_CONFIG_SHORT_ID_LENGTH 0x06
#define EEPROM_CONFIG_LONG_ID_LENGTH 0x7F


#define EEPROM_COUNTER_OFFSET 0x0E00
#define EEPROM_COUNTER_SLOTS 16

#define EEPROM_COUNTER_0_OFFSET 0x00
#define EEPROM_COUNTER_1_OFFSET 0x10
#define EEPROM_COUNTER_2_OFFSET 0x20
#define EEPROM_COUNTER_3_OFFSET 0x30
#define EEPROM_COUNTER_4_OFFSET 0x40
#define EEPROM_COUNTER_5_OFFSET 0x50
#define EEPROM_COUNTER_6_OFFSET 0x60
#define EEPROM_COUNTER_7_OFFSET 0x70
#define EEPROM_COUNTER_8_OFFSET 0x80
#define EEPROM_COUNTER_9_OFFSET 0x90
#define EEPROM_COUNTER_10_OFFSET 0xA0
#define EEPROM_COUNTER_11_OFFSET 0xB0
#define EEPROM_COUNTER_12_OFFSET 0xC0
#define EEPROM_COUNTER_13_OFFSET 0xD0
#define EEPROM_COUNTER_14_OFFSET 0xE0
#define EEPROM_COUNTER_15_OFFSET 0xF0

#define EEPROM_COUNTER_WRITES_OFFSET 0x00
#define EEPROM_COUNTER_TOTAL_FEEDS_OFFSET 0x04
#define EEPROM_COUNTER_REMAINING_PARTS_OFFSET 0x08


#define EEPROM_SPECIAL_OFFSET 0x0F00
#define EEPROM_SPECIAL_LENGTH 0x80


#define EEPROM_MAXIMUM_WRITES 1000000

#define EEPROM_PAGE_SIZE 32
#define EEPROM_ADDRESS 0b01010000
#define EEPROM_TIMEOUT 50
#define EEPROM_READ_DELAY 1
#define EEPROM_WRITE_DELAY 5

I2C_HandleTypeDef *EEPROM_hi2c;

void EEPROM_Setup(I2C_HandleTypeDef *hi2c);
void EEPROM_CheckForReInit(void);
void HW_ReInit(void);

//--Reads--
uint16_t EEPROM_ReadString(uint16_t address, char *strg, uint16_t length);
uint32_t EEPROM_ReadUint32(uint16_t address, uint32_t def);
int32_t EEPROM_ReadInt32(uint16_t address, int32_t def);
uint16_t EEPROM_ReadUint16(uint16_t address, uint16_t def);
int16_t EEPROM_ReadInt16(uint16_t address, int16_t def);
uint8_t EEPROM_ReadUint8(uint16_t address, uint8_t def);
int8_t EEPROM_ReadInt8(uint16_t address, int8_t def);

//--Writes--
uint16_t EEPROM_WriteString(uint16_t address, char *strg, uint16_t length);
uint16_t EEPROM_WriteUint32(uint16_t address, uint32_t data);
uint16_t EEPROM_WriteInt32(uint16_t address, int32_t data);
uint16_t EEPROM_WriteUint16(uint16_t address, uint16_t data);
uint16_t EEPROM_WriteInt16(uint16_t address, int16_t data);
uint16_t EEPROM_WriteUint8(uint16_t address, uint8_t data);
uint16_t EEPROM_WriteInt8(uint16_t address, int8_t data);

//--Counter--
void EEPROM_UpdateCounterSlot(void);
uint16_t EEPROM_CounterWriteUint32(uint16_t address, uint32_t data);
uint16_t EEPROM_CounterWriteInt32(uint16_t address, int32_t data);
uint16_t EEPROM_CounterCount(uint64_t totalFeeds, int32_t remainingParts);
uint32_t EEPROM_CounterReadUint32(uint16_t address, uint32_t def);
int32_t EEPROM_CounterReadInt32(uint16_t address, int32_t def);
uint64_t EEPROM_CounterCalcTotalFeeds(uint32_t readValue);

//--Util--
uint32_t EEPROM_CheckUpdateRowCounter(uint16_t address);
uint8_t EEPROM_WriteHeader(void);
uint8_t EEPROM_WriteDefaults(void);
uint8_t EEPROM_CheckHeader(void);
#endif /* INC_EEPROM_H_ */
