/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DRDY_Pin GPIO_PIN_2
#define DRDY_GPIO_Port GPIOE
#define CS_I2C_SPI_Pin GPIO_PIN_3
#define CS_I2C_SPI_GPIO_Port GPIOE
#define MEMS_INT3_Pin GPIO_PIN_4
#define MEMS_INT3_GPIO_Port GPIOE
#define MEMS_INT4_Pin GPIO_PIN_5
#define MEMS_INT4_GPIO_Port GPIOE
#define MOTOR9_DIR_Pin GPIO_PIN_15
#define MOTOR9_DIR_GPIO_Port GPIOC
#define MOTOR9_EN_Pin GPIO_PIN_10
#define MOTOR9_EN_GPIO_Port GPIOF
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define MOTOR9_PUL_Pin GPIO_PIN_1
#define MOTOR9_PUL_GPIO_Port GPIOF
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_8
#define LD4_GPIO_Port GPIOE
#define LD3_Pin GPIO_PIN_9
#define LD3_GPIO_Port GPIOE
#define LD5_Pin GPIO_PIN_10
#define LD5_GPIO_Port GPIOE
#define LD7_Pin GPIO_PIN_11
#define LD7_GPIO_Port GPIOE
#define LD9_Pin GPIO_PIN_12
#define LD9_GPIO_Port GPIOE
#define LD10_Pin GPIO_PIN_13
#define LD10_GPIO_Port GPIOE
#define LD8_Pin GPIO_PIN_14
#define LD8_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOE
#define MOTOR5_EN_Pin GPIO_PIN_14
#define MOTOR5_EN_GPIO_Port GPIOB
#define MOTOR2_DIR_Pin GPIO_PIN_15
#define MOTOR2_DIR_GPIO_Port GPIOB
#define MOTOR5_PUL_Pin GPIO_PIN_8
#define MOTOR5_PUL_GPIO_Port GPIOD
#define MOTOR2_PUL_Pin GPIO_PIN_9
#define MOTOR2_PUL_GPIO_Port GPIOD
#define MOTOR5_DIR_Pin GPIO_PIN_10
#define MOTOR5_DIR_GPIO_Port GPIOD
#define MOTOR2_EN_Pin GPIO_PIN_11
#define MOTOR2_EN_GPIO_Port GPIOD
#define MOTOR4_EN_Pin GPIO_PIN_12
#define MOTOR4_EN_GPIO_Port GPIOD
#define MOTOR1_EN_Pin GPIO_PIN_13
#define MOTOR1_EN_GPIO_Port GPIOD
#define MOTOR4_PUL_Pin GPIO_PIN_14
#define MOTOR4_PUL_GPIO_Port GPIOD
#define MOTOR1_PUL_Pin GPIO_PIN_15
#define MOTOR1_PUL_GPIO_Port GPIOD
#define MOTOR1_DIR_Pin GPIO_PIN_6
#define MOTOR1_DIR_GPIO_Port GPIOC
#define MOTOR4_DIR_Pin GPIO_PIN_7
#define MOTOR4_DIR_GPIO_Port GPIOC
#define MOTOR6_EN_Pin GPIO_PIN_8
#define MOTOR6_EN_GPIO_Port GPIOC
#define MOTOR7_PUL_Pin GPIO_PIN_9
#define MOTOR7_PUL_GPIO_Port GPIOC
#define MOTOR6_PUL_Pin GPIO_PIN_8
#define MOTOR6_PUL_GPIO_Port GPIOA
#define MOTOR7_DIR_Pin GPIO_PIN_9
#define MOTOR7_DIR_GPIO_Port GPIOA
#define MOTOR6_DIR_Pin GPIO_PIN_10
#define MOTOR6_DIR_GPIO_Port GPIOA
#define MOTOR7_EN_Pin GPIO_PIN_11
#define MOTOR7_EN_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define MOTOR8_EN_Pin GPIO_PIN_15
#define MOTOR8_EN_GPIO_Port GPIOA
#define MOTOR8_PUL_Pin GPIO_PIN_12
#define MOTOR8_PUL_GPIO_Port GPIOC
#define MOTOR8_DIR_Pin GPIO_PIN_0
#define MOTOR8_DIR_GPIO_Port GPIOD
#define MOTOR3_DIR_Pin GPIO_PIN_2
#define MOTOR3_DIR_GPIO_Port GPIOD
#define MOTOR3_PUL_Pin GPIO_PIN_4
#define MOTOR3_PUL_GPIO_Port GPIOD
#define MOTOR3_EN_Pin GPIO_PIN_6
#define MOTOR3_EN_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_7
#define I2C1_SDA_GPIO_Port GPIOB
#define MEMS_INT1_Pin GPIO_PIN_0
#define MEMS_INT1_GPIO_Port GPIOE
#define MEMS_INT2_Pin GPIO_PIN_1
#define MEMS_INT2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
