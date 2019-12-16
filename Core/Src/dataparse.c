/*
 * dataparse.c
 *
 *  Created on: 11 но€б. 2019 г.
 *      Author: Andy
 */

#include "dataparse.h"
#include "stdlib.h"
#include "stdio.h"
#include <math.h>
#include "stm32f3xx.h"
#include "usart.h"
#include "div268n.h"
#include "eeprom.h"
#include "correction.h"

#define M_PI 3.14159265358979323846
#define COIL_DIAM 19.6

float _ConvMas[] =
{ COIL_DIAM * M_PI / 3200, COIL_DIAM * M_PI / 3200, COIL_DIAM * M_PI / 3200,
		COIL_DIAM * M_PI / 3200,
		COIL_DIAM * M_PI / 3200, COIL_DIAM * M_PI / 3200, COIL_DIAM * M_PI
				/ 3200, COIL_DIAM * M_PI / 3200, 5.0 / 3200 };

float _convert_slice(const char *s, int a, int b)
{
	char buf[10];
	for (int i = 0; i < 10; i++)
		buf[i] = 0;
	if (b - a < 10)
	{
		for (int pointer = 0; pointer < b - a; pointer++)
		{
			if ((s[a + pointer] - 47 > 0 && s[a + pointer] - 47 < 11)
					|| s[a + pointer] == '.' || s[a + pointer] == '-')
				buf[pointer] = s[a + pointer];
			else
				return (0);
		}
	}
	else
	{
		HAL_UART_Transmit(&huart1, (uint8_t*) "t", 1, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart4, (uint8_t*) "Wrong number!\r\n", 15,
		HAL_MAX_DELAY);
		return (0);
	}
	float temp = atoff(buf);
	return (temp);
}

int _USER_Data_InsertToMassive(char buffer[], int pointer, int end,
		int32_t mas[], int offset)
{

	if (buffer[pointer] != 'c')
	{
		offset = offset + buffer[pointer + 1] - '1';
		pointer += 3;
	}
	else
		pointer += 2;
	int numEnd = pointer;
	while (buffer[numEnd] != ' ' && numEnd < end)
	{
		numEnd++;
	}
	float temp = _convert_slice(buffer, pointer, numEnd);
	int32_t temp2 = round(temp / _ConvMas[offset]);
	if (offset < 9 && abs(temp2) < 100000)
	{
		mas[offset] = temp2;
		pointer = numEnd + 1;
		return (pointer);
	}
	else
	{
		HAL_UART_Transmit(&huart1, (uint8_t*) "t", 1, HAL_MAX_DELAY);
		return (999);
	}
}

void USER_Data_Parse_Old(char buffer[], int lastIndex, int32_t mas[])
{
	uint16_t pointer = 0;
	while (pointer < lastIndex)
	{
		switch (buffer[pointer])
		{
		case 'r':
			pointer = _USER_Data_InsertToMassive(buffer, pointer, lastIndex,
					mas, 0);
			break;
		case 'l':
			pointer = _USER_Data_InsertToMassive(buffer, pointer, lastIndex,
					mas, 4);
			break;
		case 'c':
			pointer = _USER_Data_InsertToMassive(buffer, pointer, lastIndex,
					mas, 8);
		default:
			pointer++;
			break;
		}
	}
	if (mas[8] != 0)
	{
		MoveCorr(mas, _ConvMas, DIV268N_ShowSteps(8));
		for (int i = 0; i < 8; i++) //Set new speeds of each driver
		{
			if (mas[i] != 0)
			{
				float speed = SpeedCorr(mas, i);
				if (speed < 12.0) //check over speed
					DIV268N_WriteSpeed(i, speed);
			}
		}
	}
	DIV268N_Move(mas);
	for (int i = 0; i < 8; i++) //Set new speeds of each driver
	{
		DIV268N_WriteSpeed(i, 4.6875);
	}
}

void USER_Data_Parse_Command()
{

}

void USER_Data_Parse_Movement(char BufChar[], uint16_t pointer,
		int32_t inputMas[])
{
	BufChar[0] = 'M';
	int32_t checkSum = 0;
	sscanf(BufChar, "M%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\\", &inputMas[0],
			&inputMas[1], &inputMas[2], &inputMas[3], &inputMas[4],
			&inputMas[5], &inputMas[6], &inputMas[7], &inputMas[8],
			&inputMas[9]);
	for (int i = 0; i < 9; i++)
		checkSum += inputMas[i];
	if (checkSum == inputMas[9])
	{
		for (int i = 0; i < 9; i++)
			inputMas[i] = inputMas[i] - DIV268N_ShowSteps(i);
		if (inputMas[8] != 0)
		{
			for (int i = 0; i < 8; i++) //Set new speeds of each driver
			{
				if (inputMas[i] != 0)
				{
					float speed = SpeedCorr(inputMas, i);
					if (speed < 12.0) //check over speed
						DIV268N_WriteSpeed(i, speed);
				}
			}
		}
		DIV268N_Move(inputMas);
		for (int i = 0; i < 8; i++) //Set new speeds of each driver
		{
			DIV268N_WriteSpeed(6, i);
		}
		HAL_UART_Transmit(&huart1, (uint8_t*) "s", 1, HAL_MAX_DELAY);
	}
	else
		HAL_UART_Transmit(&huart1, (uint8_t*) "t", 1, HAL_MAX_DELAY);

}

void USER_DataPasre_ManualControl(char BufChar[], uint16_t pointer,
		int32_t inputMas[])
{
	sscanf(BufChar, "H%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\\", &inputMas[0],
			&inputMas[1], &inputMas[2], &inputMas[3], &inputMas[4],
			&inputMas[5], &inputMas[6], &inputMas[7], &inputMas[8],
			&inputMas[9]);
	int32_t checkSum = 0;
	for (int i = 0; i < 9; i++)
		checkSum += inputMas[i];
	if (checkSum == inputMas[9])
	{
		for (int i = 0; i < 9; i++)
			inputMas[i] = inputMas[i] - DIV268N_ShowSteps(i);
		DIV268N_Move(inputMas);
		HAL_UART_Transmit(&huart1, (uint8_t*) "s", 1, HAL_MAX_DELAY);
	}
	else
		HAL_UART_Transmit(&huart1, (uint8_t*) "t", 1, HAL_MAX_DELAY);
}

void USER_Data_Parse_Analys(char BufChar[], uint16_t pointer,
		int32_t inputMas[])
{
	if (BufChar[0] == 'W')
	{
		HAL_UART_Transmit(&huart1, (uint8_t*) "Test stand MATLAB output\r\n",
				26, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart4, (uint8_t*) "Test stand DEBUG output\r\n", 25,
		HAL_MAX_DELAY);
	}
	else if (BufChar[0] == '/' || BufChar[0] == 'M')
	{
		USER_Data_Parse_Movement(BufChar, pointer, inputMas);
	}
	else if (BufChar[0] == 'H')
	{
		USER_DataPasre_ManualControl(BufChar, pointer, inputMas);
	}
	else if (BufChar[0] == 'B')
	{
		for (int i = 0; i < 9; i++)
			inputMas[i] = -DIV268N_ShowSteps(i);
		DIV268N_Move(inputMas);
	}
	else if (BufChar[0] == 'Z')
	{
		if (pointer < 5)
		{
			for (int i = 0; i < 8; i++)
				DIV268N_WriteSteps(i, 0);
		}
		else
		{
			for (int i = 0; i < 9; i++)
				DIV268N_WriteSteps(i, 0);
		}
	}
	else if (BufChar[0] == 'r' || BufChar[0] == 'l' || BufChar[0] == 'c')
	{
		USER_Data_Parse_Old(BufChar, pointer, inputMas);
	}
	else
		HAL_UART_Transmit(&huart1, (uint8_t*) "t", 1, HAL_MAX_DELAY);
}
