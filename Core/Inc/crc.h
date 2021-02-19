/*
 * crc.h
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
 *  Created on: Jan. 24, 2021
 *      Author: SG-O
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_

#include "main.h"
#include "conversion.h"

#define CRC_CONDITIONED_LENGTH 64

CRC_HandleTypeDef *CRC_hcrc;

void CRC_Start(CRC_HandleTypeDef *hcrc);
uint8_t CRC_Get16H(void);
uint8_t CRC_Calculate8L(uint8_t *input, uint8_t inputLength);
uint32_t CRC_Calculate32(uint8_t *input, uint8_t inputLength);

#endif /* INC_CRC_H_ */
