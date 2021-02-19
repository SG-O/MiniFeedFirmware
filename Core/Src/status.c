/*
 * status.c
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
 *  Created on: Jan. 20, 2021
 *      Author: SG-O
 */

#include "status.h"

uint8_t STATUS_status = STATUS_BUSY;
uint8_t ERROR_error = ERROR_NO_ERROR;

uint8_t STATUS_GetStatus() {
	return STATUS_status;
}

uint8_t ERROR_GetError() {
	return ERROR_error;
}

void ERROR_SetError(uint8_t errorCode) {
	if (errorCode == ERROR_NO_ERROR) {
		ERROR_ResetError();
		return;
	}
	STATUS_status = STATUS_ERROR;
	ERROR_error = errorCode;
}

void ERROR_ResetError() {
	STATUS_status = STATUS_READY;
	ERROR_error = ERROR_NO_ERROR;
}

void STATUS_SetReady() {
	if (STATUS_status == STATUS_ERROR) return;
	STATUS_status = STATUS_READY;
}

void STATUS_SetBusy() {
	if (STATUS_status == STATUS_ERROR) return;
	STATUS_status = STATUS_BUSY;
}
