/*
 * DIV268N_StandConfigLib.c
 *
 *  Created on: 7 окт. 2019 г.
 *      Author: Andy
 */
#include "div268n.h"
#include "stdlib.h"
#include "main.h"
#include "stm32f3xx.h"
#include "serial.h"
#include "math.h"

struct _driverParametersList // structure with all states that needed for driver
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	GPIO_TypeDef* GPIOxDir;
	uint16_t GPIO_Pin_Dir;
	GPIO_TypeDef* GPIOxEnb;
	uint16_t GPIO_Pin_Enb;
	uint8_t microStep; //Set microstep of the engine, which set on Driver
	uint8_t dir; //byte, if =1 - inverted rolling, 0 - standard
	uint8_t currDir; //Current state of DirPin
};

struct _driverParametersList _motors[9];
uint8_t _numberOfDrivers = 9;
int32_t _stepsCount[9];
uint8_t _state[9];
uint32_t _pulse[9];
uint32_t _time[9];

void _DIV268N_DirSetReset(uint8_t _pin, uint8_t _state)
{
	switch (_pin)
	{
	case 0:
		HAL_GPIO_WritePin(MOTOR1_DIR_GPIO_Port, MOTOR1_DIR_Pin, _state);
		break;
	case 1:
		HAL_GPIO_WritePin(MOTOR2_DIR_GPIO_Port, MOTOR2_DIR_Pin, _state);
		break;
	case 2:
		HAL_GPIO_WritePin(MOTOR3_DIR_GPIO_Port, MOTOR3_DIR_Pin, _state);
		break;
	case 3:
		HAL_GPIO_WritePin(MOTOR4_DIR_GPIO_Port, MOTOR4_DIR_Pin, _state);
		break;
	case 4:
		HAL_GPIO_WritePin(MOTOR5_DIR_GPIO_Port, MOTOR5_DIR_Pin, _state);
		break;
	case 5:
		HAL_GPIO_WritePin(MOTOR6_DIR_GPIO_Port, MOTOR6_DIR_Pin, _state);
		break;
	case 6:
		HAL_GPIO_WritePin(MOTOR7_DIR_GPIO_Port, MOTOR7_DIR_Pin, _state);
		break;
	case 7:
		HAL_GPIO_WritePin(MOTOR8_DIR_GPIO_Port, MOTOR8_DIR_Pin, _state);
		break;
	case 8:
		HAL_GPIO_WritePin(MOTOR9_DIR_GPIO_Port, MOTOR9_DIR_Pin, _state);
		break;
	default:
		break;
	}
}

void _DIV268N_PinSet(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->BSRR = (uint32_t) GPIO_Pin;
}

void _DIV268N_PinReset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->BRR = (uint32_t) GPIO_Pin;
}

void _DIV268N_PrepareMove(int32_t *_stepsInput)
{
	for (uint8_t i = 0; i < _numberOfDrivers; i++)
	{
		_stepsCount[i] += _stepsInput[i]; //Add movement to massive of current positions
		if (_stepsInput[i] < 0)
		{
			_motors[i].currDir = -1; //set state of inverted work
		}
		else
		{
			_motors[i].currDir = 1; //set state of normal work
		}
	}
	//Inverse data if needed
	for (uint8_t i = 0; i < _numberOfDrivers; i++)
	{
		if (_motors[i].dir == 1)
		{
			_stepsInput[i] = _stepsInput[i] * (-1); //to set another direction of move if necessary
		}
	}
	//Set direction to drivers and make all steps >0
	for (uint8_t i = 0; i < _numberOfDrivers; i++)
	{
		if (_stepsInput[i] < 0)
		{
			//_motors[i].currDir = -1; //set state of inverted work
			_stepsInput[i] = abs(_stepsInput[i]);  //module of steps count
			_DIV268N_DirSetReset(i, GPIO_PIN_SET); //Set state of dirPin, to set direction of movement
		}
		else
		{
			//_motors[i].currDir = 1; //set state of normal work
			_DIV268N_DirSetReset(i, GPIO_PIN_RESET); //so LOW signal on direction pin
		}
	}
}

void _DIV268N_MoveRL(int32_t *_stepsInputConv)
{
	TIM2->CNT = 0x00000000; //—брос регистра в ноль воизбежание переполнени€
	uint32_t s = 0;
	uint32_t tm = (TIM2->CNT) + 50;
	for (uint8_t i = 0; i < 9; i++)
	{
		_time[i] = tm;
		s = s + _stepsInputConv[i];
	}
	while (s > 0)
	{
		tm = TIM2->CNT;
		for (uint8_t i = 0; i < _numberOfDrivers; i++)
		{
			if (_time[i] <= (tm))
			{
				if (_stepsInputConv[i] > 0)
				{
					if (_state[i] == 0)
					{
						_DIV268N_PinSet(_motors[i].GPIOx, _motors[i].GPIO_Pin);
						_state[i] = ~_state[i]; //changing state - motor in work, high cycle
						_time[i] = _time[i] + _pulse[i]; //add next timer alarm
					}
					else
					{
						_DIV268N_PinReset(_motors[i].GPIOx,
								_motors[i].GPIO_Pin);
						_state[i] = ~_state[i]; //changing state of motor - motor work, low cycle
						_time[i] += _pulse[i]; //add next timer alarm
						_stepsInputConv[i]--; //lowering count of steps
						s--;
					}
				}
			}
		}
		if (SERIAL_UART1_BytesAvailable() != 0 || SERIAL_UART4_BytesAvailable() != 0) //if we founding data in serial port if (Serial.available() != 0 || Serial2.available() != 0)
		{ //we writing our real position
			for (uint8_t i = 0; i < _numberOfDrivers; i++)
			{
				if (_motors[i].currDir == 1) //if motor no inverted
					_stepsCount[i] = _stepsCount[i] - (_stepsInputConv[i]); //standard, * because of direction of motor
				else
					_stepsCount[i] = _stepsCount[i] + (_stepsInputConv[i]); //if inverted
				_stepsInputConv[i] = 0; //Movement massive =0; no need to move engines again
				s = 0;
			}
		}
	}
}

void DIV268N_Init(void)
{
	for (uint8_t i = 0; i < 9; i++)
	{
		_motors[i].microStep = 16;
		_pulse[i] = 2000 * 8;//937 * 8;
		_state[i] = 0;
		_motors[i].dir = 0;
		_motors[i].currDir = 0;
		_time[i] = 0;
		_stepsCount[i] = 0;
	}
	_motors[2].dir = 1;
	_motors[3].dir = 1;
	_motors[4].dir = 1;
	_motors[5].dir = 1;
	_motors[8].dir = 1;
	//«апись данных о портах в структуру
	_motors[0].GPIOx = MOTOR1_PUL_GPIO_Port;
	_motors[0].GPIO_Pin = MOTOR1_PUL_Pin;
	_motors[1].GPIOx = MOTOR2_PUL_GPIO_Port;
	_motors[1].GPIO_Pin = MOTOR2_PUL_Pin;
	_motors[2].GPIOx = MOTOR3_PUL_GPIO_Port;
	_motors[2].GPIO_Pin = MOTOR3_PUL_Pin;
	_motors[3].GPIOx = MOTOR4_PUL_GPIO_Port;
	_motors[3].GPIO_Pin = MOTOR4_PUL_Pin;
	_motors[4].GPIOx = MOTOR5_PUL_GPIO_Port;
	_motors[4].GPIO_Pin = MOTOR5_PUL_Pin;
	_motors[5].GPIOx = MOTOR6_PUL_GPIO_Port;
	_motors[5].GPIO_Pin = MOTOR6_PUL_Pin;
	_motors[6].GPIOx = MOTOR7_PUL_GPIO_Port;
	_motors[6].GPIO_Pin = MOTOR7_PUL_Pin;
	_motors[7].GPIOx = MOTOR8_PUL_GPIO_Port;
	_motors[7].GPIO_Pin = MOTOR8_PUL_Pin;
	_motors[8].GPIOx = MOTOR9_PUL_GPIO_Port;
	_motors[8].GPIO_Pin = MOTOR9_PUL_Pin;
	_motors[0].GPIOxDir = MOTOR1_DIR_GPIO_Port;
	_motors[0].GPIOxEnb = MOTOR1_EN_GPIO_Port;
	_motors[1].GPIOxDir = MOTOR2_DIR_GPIO_Port;
	_motors[1].GPIOxEnb = MOTOR2_EN_GPIO_Port;
	_motors[2].GPIOxDir = MOTOR3_DIR_GPIO_Port;
	_motors[2].GPIOxEnb = MOTOR3_EN_GPIO_Port;
	_motors[3].GPIOxDir = MOTOR4_DIR_GPIO_Port;
	_motors[3].GPIOxEnb = MOTOR4_EN_GPIO_Port;
	_motors[4].GPIOxDir = MOTOR5_DIR_GPIO_Port;
	_motors[4].GPIOxEnb = MOTOR5_EN_GPIO_Port;
	_motors[5].GPIOxDir = MOTOR6_DIR_GPIO_Port;
	_motors[5].GPIOxEnb = MOTOR6_EN_GPIO_Port;
	_motors[6].GPIOxDir = MOTOR7_DIR_GPIO_Port;
	_motors[6].GPIOxEnb = MOTOR7_EN_GPIO_Port;
	_motors[7].GPIOxDir = MOTOR8_DIR_GPIO_Port;
	_motors[7].GPIOxEnb = MOTOR8_EN_GPIO_Port;
	_motors[8].GPIOxDir = MOTOR9_DIR_GPIO_Port;
	_motors[8].GPIOxEnb = MOTOR9_EN_GPIO_Port;
	_motors[0].GPIO_Pin_Dir = MOTOR1_DIR_Pin;
	_motors[0].GPIO_Pin_Enb = MOTOR1_EN_Pin;
	_motors[1].GPIO_Pin_Dir = MOTOR2_DIR_Pin;
	_motors[1].GPIO_Pin_Enb = MOTOR2_EN_Pin;
	_motors[2].GPIO_Pin_Dir = MOTOR3_DIR_Pin;
	_motors[2].GPIO_Pin_Enb = MOTOR3_EN_Pin;
	_motors[3].GPIO_Pin_Dir = MOTOR4_DIR_Pin;
	_motors[3].GPIO_Pin_Enb = MOTOR4_EN_Pin;
	_motors[4].GPIO_Pin_Dir = MOTOR5_DIR_Pin;
	_motors[4].GPIO_Pin_Enb = MOTOR5_EN_Pin;
	_motors[5].GPIO_Pin_Dir = MOTOR6_DIR_Pin;
	_motors[5].GPIO_Pin_Enb = MOTOR6_EN_Pin;
	_motors[6].GPIO_Pin_Dir = MOTOR7_DIR_Pin;
	_motors[6].GPIO_Pin_Enb = MOTOR7_EN_Pin;
	_motors[7].GPIO_Pin_Dir = MOTOR8_DIR_Pin;
	_motors[7].GPIO_Pin_Enb = MOTOR8_EN_Pin;
	_motors[8].GPIO_Pin_Dir = MOTOR9_DIR_Pin;
	_motors[8].GPIO_Pin_Enb = MOTOR9_EN_Pin;
	_pulse[8] = 134 * 8;
}

void DIV268N_Move(int32_t *_stepsInput)
{
	_DIV268N_PrepareMove(_stepsInput);
	_DIV268N_MoveRL(_stepsInput);
}

int32_t DIV268N_ShowSteps(uint8_t i)
{ //show steps of current motor from internal counter
	return (_stepsCount[i]);
}

void DIV268N_WriteSteps(uint8_t i, int32_t value)
{ //show steps of current motor from internal counter
	_stepsCount[i] = value;
}

void DIV268N_WriteSpeed(uint8_t i, float value)
{
	_pulse[i]=round(150000.0 / (_motors[i].microStep * value)*8);
}
