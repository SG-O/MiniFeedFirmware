/*
 * com.c
 *
 *  Created on: Jan. 21, 2021
 *      Author: SG-O
 */

#include "com.h"

uint8_t buf[256];
uint8_t ignore[1];
uint8_t resultLength = 0;
uint8_t dir = 0;
uint8_t transaction = 0;

uint8_t opcode;
uint8_t value;
uint8_t length;

uint8_t crcL;
uint8_t crcH;

void COM_ClearBuf(uint8_t length) {
	for(int i = 1; i < length; i++) {
		buf[i] = 0;
	}
}

void COM_ParseBuffer() {
	if (buf[0] == OPCODE_READ_LONG || buf[0] == OPCODE_READ_SHORT) {
		length = 0;
		opcode = buf[0];
		value = buf[1];
	} else if (buf[0] > 127) {
		length = 0;
		opcode = buf[0];
		crcL = buf[1];
		crcH = 0;
	} else {
		length = buf[0];
		opcode = buf[1];
		crcH = buf[length + 2];
		crcL = buf[length + 3];
	}
}

uint8_t COM_CompareCRC() {
	if (opcode == OPCODE_READ_LONG || opcode == OPCODE_READ_SHORT) return 1;
	if (opcode > 127) {
		uint8_t calc[] = {COM_address, opcode};
		uint8_t calcL = CRC_Calculate8L(calc, 2);
		if (calcL == crcL) return 1;
	} else {
		uint8_t calc[length + 3];
		calc[0] = COM_address;
		calc[1] = length;
		calc[2] = opcode;
		for (int i = 0; i < length; i++) {
			calc[i + 3] = buf[i+2];
		}
		uint8_t calcL = CRC_Calculate8L(calc, length + 3);
		uint8_t calcH = CRC_Get16H();
		if (calcL == crcL && calcH == crcH) return 1;
	}
	return 0;
}

void COM_PutUnsigned8(uint8_t out) {
	buf[resultLength] = out;
	resultLength ++;
}

void COM_PutUnsigned16(uint16_t out) {
	CON_Unsigned16ToBytes(out, &buf[resultLength], 2);
	resultLength += 2;
}

void COM_PutSigned16(int16_t out) {
	CON_Signed16ToBytes(out, &buf[resultLength], 2);
	resultLength += 2;
}

void COM_PutUnsigned32(uint32_t out) {
	CON_Unsigned32ToBytes(out, &buf[resultLength], 4);
	resultLength += 4;
}

void COM_PutSigned32(int32_t out) {
	CON_Signed32ToBytes(out, &buf[resultLength], 4);
	resultLength += 4;
}

void COM_GenerateLongCRC() {
	uint8_t calc[resultLength + 3];
	calc[0] = COM_address;
	calc[1] = OPCODE_READ_LONG;
	calc[2] = value;
	for (int i = 0; i < resultLength; i++) {
		calc[i + 3] = buf[i];
	}
	uint8_t calcL = CRC_Calculate8L(calc, resultLength + 3);
	uint8_t calcH = CRC_Get16H();
	COM_PutUnsigned8(calcH);
	COM_PutUnsigned8(calcL);
}

void COM_GenerateShortCRC() {
	uint8_t calc[] = {COM_address, OPCODE_READ_SHORT, value, buf[0]};
	uint8_t calcL = CRC_Calculate8L(calc, 4);
	COM_PutUnsigned8(calcL);
}

void COM_parseOpcode() {
	buf[0] = OPCODE_Parse(opcode, length, &buf[2]);
	resultLength = 1;
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (dir == 1) {
		COM_ClearBuf(resultLength);
		resultLength = 0;
	} else {

	}
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	transaction = 0;
	if( TransferDirection==I2C_DIRECTION_TRANSMIT ) {
		dir = 0;
		buf[0] = 128;
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, buf, 1, I2C_NEXT_FRAME);
	} else {
		dir = 1;
		if (resultLength < 1) {
			ignore[0] = 1;
			HAL_I2C_Slave_Seq_Transmit_IT(hi2c, ignore, 1, I2C_NEXT_FRAME);
		} else {
			HAL_I2C_Slave_Seq_Transmit_IT(hi2c, buf, resultLength, I2C_NEXT_FRAME);
		}
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	uint8_t rxLength;
	if (transaction == 0) {
		if (buf[0] > 252 && buf[0] < 255) {
			rxLength = 1;
		} else if (buf[0] > 127) {
			rxLength = 1;
		} else {
			rxLength = buf[0] + 3;
		}
		COM_ClearBuf(rxLength);
		transaction = 1;
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, &buf[1], rxLength, I2C_NEXT_FRAME);
	} else {
		COM_ParseBuffer();
		uint8_t checkResult = COM_CompareCRC();
		if (checkResult == 1) {
			if (opcode == OPCODE_READ_LONG || opcode == OPCODE_READ_SHORT) {
				if (opcode == OPCODE_READ_LONG) {
					VALUE_ParseLongRead(value);
				} else {
					VALUE_ParseShortRead(value);
				}
			} else {
				COM_parseOpcode();
			}
		} else {
			ERROR_SetError(ERROR_CRC);
			buf[0] = ERROR_GetError();
			resultLength = 1;
		}
		length = 0;
		opcode = 128;
		HAL_I2C_EnableListen_IT(hi2c);
	}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	ignore[0] = 0;
	HAL_I2C_Slave_Seq_Transmit_IT(hi2c, ignore, 1, I2C_NEXT_FRAME);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	if( HAL_I2C_GetError(hi2c)==HAL_I2C_ERROR_AF) {
	} else {
	}
}

uint8_t COM_GetAddress() {
	return COM_address;
}

void COM_SetAddress() {
	uint8_t i = 0;
	if (COM_hi2c->State != HAL_I2C_STATE_READY) {
		if (i > 100) return;
		HAL_I2C_DisableListen_IT(COM_hi2c);
		HAL_Delay(10);
		i++;
	}
	uint8_t persAddress = PERSIST_GetI2CAddress();
	uint8_t address = ANALOG_GetSlot() + 16;
	if (persAddress < 0x50) {
		address = persAddress;
	}
	if (address > 0x4F) {
		ERROR_SetError(ERROR_INITIALISATION_FAILED);
		return;
	}
	COM_address = address;
	COM_hi2c->State = HAL_I2C_STATE_BUSY;
	__HAL_I2C_DISABLE(COM_hi2c);
	COM_hi2c->Instance->OAR1 &= ~I2C_OAR1_OA1EN;
	if (COM_hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
	{
		COM_hi2c->Instance->OAR1 = (I2C_OAR1_OA1EN | (address << 1));
	}
	__HAL_I2C_ENABLE(COM_hi2c);
	COM_hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
	COM_hi2c->State = HAL_I2C_STATE_READY;
	COM_hi2c->Mode = HAL_I2C_MODE_NONE;
	HAL_I2C_EnableListen_IT(COM_hi2c);
}

void COM_Setup(I2C_HandleTypeDef *hi2c) {
	COM_hi2c = hi2c;
	COM_SetAddress();
}
