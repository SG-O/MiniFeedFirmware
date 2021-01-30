/*
 * status.c
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
