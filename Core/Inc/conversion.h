/*
 * conversion.h
 *
 *  Created on: Jan. 24, 2021
 *      Author: SG-O
 */

#ifndef INC_CONVERSION_H_
#define INC_CONVERSION_H_

#include <stddef.h>
#include <stdint.h>

uint16_t CON_BytesToUnsigned16(uint8_t *bytes, uint8_t length);
uint32_t CON_BytesToUnsigned32(uint8_t *bytes, uint8_t length);
void CON_Unsigned16ToBytes(uint16_t input, uint8_t *bytes, uint8_t length);
void CON_Unsigned32ToBytes(uint32_t input, uint8_t *bytes, uint8_t length);
int16_t CON_BytesToSigned16(uint8_t *bytes, uint8_t length);
int32_t CON_BytesToSigned32(uint8_t *bytes, uint8_t length);
void CON_Signed16ToBytes(int16_t input, uint8_t *bytes, uint8_t length);
void CON_Signed32ToBytes(int32_t input, uint8_t *bytes, uint8_t length);

#endif /* INC_CONVERSION_H_ */
