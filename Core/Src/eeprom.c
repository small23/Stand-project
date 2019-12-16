/*
 * eeprom.c
 *
 *  Created on: 26 окт. 2019 г.
 *      Author: Andy
 */
#include "eeprom.h"
#include "stm32f3xx.h"
#include "i2c.h"

void EEPROM_WriteSteps(int32_t inputMas[])
{
	HAL_StatusTypeDef status;
	uint16_t devAddr = (0x50 << 1);
	uint16_t memAddr = 0x0;
	uint8_t raw[4];
	for (uint8_t i = 0; i < 9; i++)
	{
		memAddr = 0x0 + i * 4;
		*(int32_t*) raw = inputMas[i];
		HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT,
				(uint8_t*) raw, sizeof(uint32_t), HAL_MAX_DELAY);
		for (;;)
		{ // wait...
			status = HAL_I2C_IsDeviceReady(&hi2c1, devAddr, 1,
			HAL_MAX_DELAY);
			if (status == HAL_OK)
				break;
		}
	}
}

void EEPROM_ReadSteps(int32_t tempMas[])
{
	HAL_StatusTypeDef status;
	uint8_t rmsg[4];
	for (uint8_t i = 0; i < 9; i++)
	{
		uint16_t devAddr = (0x50 << 1);
		uint16_t memAddr = 0x0 + i * 4;
		HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT,
				(uint8_t*) rmsg, sizeof(rmsg), HAL_MAX_DELAY);
		for (;;)
		{ // wait...
			status = HAL_I2C_IsDeviceReady(&hi2c1, devAddr, 1,
			HAL_MAX_DELAY);
			if (status == HAL_OK)
				break;
		}
		tempMas[i] = *(int32_t*) rmsg;
		memAddr = memAddr + 4;
	}
}
