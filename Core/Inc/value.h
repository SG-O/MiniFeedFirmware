/*
 * value.h
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
 *  Created on: Jan. 26, 2021
 *      Author: SG-O
 */

#ifndef INC_VALUE_H_
#define INC_VALUE_H_

#include "com.h"
#include "hardware.h"

#define VALUE_FEEDER_ID 0
#define VALUE_FIRMWARE_VERSION 1
#define VALUE_HARDWARE_VERSION 2
#define VALUE_REMAINING_PARTS 3
#define VALUE_TOTAL_PARTS 4
#define VALUE_LOW_PARTS 5
#define VALUE_SHORT_ID 6
#define VALUE_LONG_ID 7
#define VALUE_MOT_SLOWDOWN 8
#define VALUE_TOTAL_FEEDS 125

#define VALUE_PROTO_VERSION 128
#define VALUE_STATUS 129
#define VALUE_ERROR 130
#define VALUE_PART_PITCH 131
#define VALUE_FEED_SPEED 132
#define VALUE_BRIGHTNESS 133
#define VALUE_MOT_DIR 134

void VALUE_ParseLongRead(uint8_t value);
void VALUE_ParseShortRead(uint8_t value);

#endif /* INC_VALUE_H_ */
