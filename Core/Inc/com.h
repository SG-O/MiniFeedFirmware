/*
 * com.h
 *
 *  Created on: Jan. 21, 2021
 *      Author: SG-O
 */

#ifndef INC_COM_H_
#define INC_COM_H_

#include "main.h"
#include "drive.h"
#include "persist.h"
#include "crc.h"
#include "hardware.h"
#include "opcode.h"
#include "value.h"

uint8_t COM_address;
I2C_HandleTypeDef *COM_hi2c;

void COM_Setup(I2C_HandleTypeDef *hi2c);
void COM_SetAddress();
uint8_t COM_GetAddress(void);
void COM_PutUnsigned8(uint8_t out);
void COM_GenerateLongCRC(void);
void COM_GenerateShortCRC(void);
void COM_PutUnsigned16(uint16_t out);
void COM_PutSigned16(int16_t out);
void COM_PutUnsigned32(uint32_t out);
void COM_PutSigned32(int32_t out);

#endif /* INC_COM_H_ */
