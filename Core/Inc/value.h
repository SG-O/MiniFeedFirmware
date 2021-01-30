/*
 * value.h
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
