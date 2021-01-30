/*
 * opcode.h
 *
 *  Created on: Jan. 26, 2021
 *      Author: SG-O
 */

#ifndef INC_OPCODE_H_
#define INC_OPCODE_H_

#include "main.h"
#include "display.h"
#include "drive.h"
#include "persist.h"
#include "conversion.h"
#include "hardware.h"

#define OPCODE_REMAINING_PARTS 0
#define OPCODE_TOTAL_PARTS 1
#define OPCODE_PART_PITCH 2
#define OPCODE_FEED_SPEED 3
#define OPCODE_LOW_PARTS 4
#define OPCODE_SHORT_ID 5
#define OPCODE_LONG_ID 6
#define OPCODE_BRIGHTNESS 7
#define OPCODE_MOT_DIR 8
#define OPCODE_MOT_SLOWDOWN 9

#define OPCODE_NOP 128
#define OPCODE_FEED 129
#define OPCODE_RESET_ERROR 130
#define OPCODE_FORCE_ERROR 131
#define OPCODE_ENTER_BOOTLOADER 251
#define OPCODE_RESET 252
#define OPCODE_READ_SHORT 253
#define OPCODE_READ_LONG 254

uint8_t OPCODE_Parse(uint8_t opcode, uint8_t length, uint8_t *data);

#endif /* INC_OPCODE_H_ */
