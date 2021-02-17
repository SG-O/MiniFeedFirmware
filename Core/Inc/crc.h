/*
 * crc.h
 *
 *  Created on: Jan. 24, 2021
 *      Author: SG-O
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_

#include "main.h"
#include "conversion.h"

CRC_HandleTypeDef *CRC_hcrc;

void CRC_Start(CRC_HandleTypeDef *hcrc);
uint8_t CRC_Get16H(void);
uint8_t CRC_Calculate8L(uint8_t *input, uint8_t inputLength);
uint32_t CRC_Calculate32(uint8_t *input, uint8_t inputLength);

#endif /* INC_CRC_H_ */
