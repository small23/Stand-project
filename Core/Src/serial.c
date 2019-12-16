/*
 * UART_Data.c
 *
 *  Created on: 25 окт. 2019 г.
 *      Author: Andy
 */
#include <serial.h>
#include "stm32f3xx_hal.h"
uint16_t _UART1_Pos = UART_BUFFER_SIZE;
uint16_t _UART4_Pos = UART_BUFFER_SIZE;
uint8_t _temp;

uint8_t SERIAL_UART1_BytesAvailable()
{
	if (_UART1_Pos != DMA1_Channel5->CNDTR)
		return (255);
	else
		return (0);
}

uint8_t SERIAL_UART1_GetByte()
{
	_temp = UART1_Buffer[UART_BUFFER_SIZE - _UART1_Pos];
	if (_UART1_Pos > 1)
		_UART1_Pos--;
	else
		_UART1_Pos = UART_BUFFER_SIZE;
	return (_temp);
}

void SERIAL_UART1_ClearBuffer()
{
	_UART1_Pos = DMA1_Channel5->CNDTR;
}

uint8_t SERIAL_UART4_BytesAvailable()
{
	if (_UART4_Pos != DMA2_Channel3->CNDTR)
		return (255);
	else
		return (0);
}

uint8_t SERIAL_UART4_GetByte()
{
	_temp = UART4_Buffer[UART_BUFFER_SIZE - _UART4_Pos];
	if (_UART4_Pos > 1)
		_UART4_Pos--;
	else
		_UART4_Pos = UART_BUFFER_SIZE;
	return (_temp);
}

void SERIAL_UART4_ClearBuffer()
{
	_UART4_Pos = DMA2_Channel3->CNDTR;
}
