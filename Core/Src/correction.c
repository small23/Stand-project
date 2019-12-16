/*
 * correction.c
 *
 *  Created on: 29 но€б. 2019 г.
 *      Author: Andy
 */

#include "correction.h"
#include "math.h"
#include "stdlib.h"
#include "stm32f3xx.h"

const float _z1 = 168.15;			//Constant length from point to engine 1/3
const float _z2 = 245.55;			//Constant length from point to engine 2/4
const float _xr1 = powf(9.54, 2); //List of constants, x - delta between axis and plate (horizontal) //5.39
const float _xr2 = powf(6.84, 2);
const float _xr3 = powf(9.54, 2);
const float _xr4 = powf(15.84, 2);
const float _xl1 = powf(15.9, 2);
const float _xl2 = powf(9.54, 2);
const float _xl3 = powf(15.9, 2);
const float _xl4 = powf(9.54, 2);
const float _yr1 = powf(55.01, 2); // y - delta between axis and engine (vertical) //53.83
const float _yr2 = powf(33.44, 2);
const float _yr3 = powf(27.93, 2);
const float _yr4 = powf(50.53, 2);
const float _yl1 = powf(52.38, 2);
const float _yl2 = powf(27.13, 2);
const float _yl3 = powf(34.3, 2);
const float _yl4 = powf(62.57, 2);

const float _a = powf(4.6, 2);
const float _const2 = 40;

float _dist[9]; //massive that contains converted length of thread, mm
float _CorrectDist[8]; //massive that contains length of correction, mm
float _Speed[9]; //massive that contains calculated speed for each engine
float _CurPos[8]; //massive that contains current length of thread
float _NewPos[8]; //massive that contains new length of thread

void DimasCorr(int32_t delta, float __diam[])  //new correction method, by Dima
{
	_CurPos[0] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8], 2) + _xr1 + _yr1)); //for each engine calculated
	_CurPos[1] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8], 2) + _xr2 + _yr2)); //current length of thread
	_CurPos[2] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8], 2) + _xr3 + _yr3)); //by Pifagor`s triangle
	_CurPos[3] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8], 2) + _xr4 + _yr4));
	_CurPos[4] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8], 2) + _xl1 + _yl1));
	_CurPos[5] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8], 2) + _xl2 + _yl2));
	_CurPos[6] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8], 2) + _xl3 + _yl3));
	_CurPos[7] = -(sqrtf((_a + powf(_const2 + delta * __diam[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8], 2) + _xl4 + _yl4));
	_NewPos[0] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8] - _dist[8], 2) + _xr1 + _yr1)); //same thing to a new position
	_NewPos[1] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8] - _dist[8], 2) + _xr2 + _yr2)); //this calculation contains calculation
	_NewPos[2] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8] - _dist[8], 2) + _xr3 + _yr3)); //of small triangle, which is the same for
	_NewPos[3] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8] - _dist[8], 2) + _xr4 + _yr4)); //all engines
	_NewPos[4] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8] - _dist[8], 2) + _xl1 + _yl1));
	_NewPos[5] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8] - _dist[8], 2) + _xl2 + _yl2));
	_NewPos[6] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z1 - delta * __diam[8] - _dist[8], 2) + _xl3 + _yl3));
	_NewPos[7] = -(sqrtf((_a + powf(_const2 + delta * __diam[8] + _dist[8], 2)))
			+ sqrtf(powf(_z2 - delta * __diam[8] - _dist[8], 2) + _xl4 + _yl4));

	for (int i = 0; i < 8; i++) //so delta = difference between
	{							//new position and current position
		_CorrectDist[i] = _NewPos[i] - _CurPos[i];
	}
}

void MoveCorr(int32_t _steps[], float _diam[], int32_t position)
{
	for (int i = 0; i < 9; i++) //converting steps to mm
	{
		_dist[i] = _diam[i] * _steps[i];
	}

	DimasCorr(position, _diam); //call the function of correction

	for (int i = 0; i < 8; i++) //add this correction to massive, which go to move function
	{
		_steps[i] += round(_CorrectDist[i] / _diam[i]); //convert mm to steps and rounding it
	}
}

float SpeedCorr(int32_t _steps[], int i) //speed correction: rpm=steps_of_curr_motor*const/steps_of_motor_numb8
{
	float res = fabsf(_steps[i] * 69.5 / _steps[8]);
	return (res); //const is calculated by god, IDKHow
}
