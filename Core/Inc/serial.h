/*
 * UART_Data.h
 *
 *  Created on: 25 окт. 2019 г.
 *      Author: Andy
 */

#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_
#include "stdio.h"

uint8_t SERIAL_UART1_BytesAvailable();
uint8_t SERIAL_UART1_GetByte();
void SERIAL_UART1_ClearBuffer();
uint8_t SERIAL_UART4_BytesAvailable();
uint8_t SERIAL_UART4_GetByte();
void SERIAL_UART4_ClearBuffer();

#define UART_BUFFER_SIZE 128
char UART1_Buffer[UART_BUFFER_SIZE];
char UART4_Buffer[UART_BUFFER_SIZE];

#endif /* INC_SERIAL_H_ */
