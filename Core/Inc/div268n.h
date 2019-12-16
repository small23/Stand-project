/*
 * DIV268N_StandConfigLib.h
 *
 *  Created on: 7 окт. 2019 г.
 *      Author: Andy
 */

#ifndef INC_DIV268N_H_
#define INC_DIV268N_H_

#include "stdio.h"

void DIV268N_Init();
void DIV268N_Move(int32_t *_stepsInput);
int32_t DIV268N_ShowSteps(uint8_t i);
void DIV268N_WriteSteps(uint8_t i, int32_t value);
void DIV268N_WriteSpeed(uint8_t i, float value);
#endif /* INC_DIV268N_H_ */
