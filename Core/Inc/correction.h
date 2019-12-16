/*
 * correction.h
 *
 *  Created on: 29 но€б. 2019 г.
 *      Author: Andy
 */

#ifndef INC_CORRECTION_H_
#define INC_CORRECTION_H_
#include "stdio.h"

float SpeedCorr(int32_t _steps[], int i);
void MoveCorr(int32_t _steps[], float _diam[], int32_t position);

#endif /* INC_CORRECTION_H_ */
