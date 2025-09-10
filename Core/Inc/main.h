/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h5xx_hal.h"

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
#define IMU_INT2_Pin GPIO_PIN_2
#define IMU_INT2_GPIO_Port GPIOE
#define AS3935_INT_Pin GPIO_PIN_3
#define AS3935_INT_GPIO_Port GPIOE
#define RED_Pin GPIO_PIN_4
#define RED_GPIO_Port GPIOE
#define GREEN_Pin GPIO_PIN_5
#define GREEN_GPIO_Port GPIOE
#define ETH_IRQ_Pin GPIO_PIN_6
#define ETH_IRQ_GPIO_Port GPIOE
#define IMU_INT1_Pin GPIO_PIN_13
#define IMU_INT1_GPIO_Port GPIOC
#define ETH_RESET__Pin GPIO_PIN_0
#define ETH_RESET__GPIO_Port GPIOF
#define ETH_GPIO_Pin GPIO_PIN_1
#define ETH_GPIO_GPIO_Port GPIOF
#define ETH_RX_ER_Pin GPIO_PIN_2
#define ETH_RX_ER_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
