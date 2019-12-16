/*
 * eeprom.h
 *
 *  Created on: 26 окт. 2019 г.
 *      Author: Andy
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "stdio.h"

void EEPROM_WriteSteps(int32_t inputMas[]);
void EEPROM_ReadSteps(int32_t tempMas[]);

#endif /* INC_EEPROM_H_ */
