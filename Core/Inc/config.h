/*
 * config.h
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
 *  Created on: Jan. 21, 2021
 *      Author: SG-O
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_PROTO_VERSION 0x01

#define CONFIG_DEFAULT_PART_PITCH 4
#define CONFIG_DEFAULT_FEED_SPEED 255
#define CONFIG_DEFAULT_DISPLAY_BRIGHTNESS 255
#define CONFIG_DEFAULT_MOTOR_DIRECTION 0
#define CONFIG_DEFAULT_MOTOR_SLOWDOWN_DELAY 50
#define CONFIG_DEFAULT_REMAINING_PARTS 0
#define CONFIG_DEFAULT_TOTAL_PARTS 0
#define CONFIG_DEFAULT_LOW_PARTS_WARN 0
#define CONFIG_DEFAULT_LONG_PARTS_ID "Not set"

#define CONFIG_ENABLE_WDT

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif

#ifndef VERSION_MINOR
#define VERSION_MINOR 0
#endif

#ifndef VERSION_PATCH
#define VERSION_PATCH 0
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */
