/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config.h"
#include "lang.h"
#include "status.h"
#include "ui.h"
#include "analog.h"
#include "backup.h"
#include <stdio.h>
#include <string.h>
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Button_B_Pin GPIO_PIN_9
#define Button_B_GPIO_Port GPIOB
#define Button_B_EXTI_IRQn EXTI4_15_IRQn
#define HW_MV_Pin GPIO_PIN_14
#define HW_MV_GPIO_Port GPIOC
#define M_EN_Pin GPIO_PIN_0
#define M_EN_GPIO_Port GPIOA
#define M_PH_Pin GPIO_PIN_1
#define M_PH_GPIO_Port GPIOA
#define M_Sleep_Pin GPIO_PIN_2
#define M_Sleep_GPIO_Port GPIOA
#define M_ISense_Pin GPIO_PIN_3
#define M_ISense_GPIO_Port GPIOA
#define M_Fault_Pin GPIO_PIN_4
#define M_Fault_GPIO_Port GPIOA
#define M_Fault_EXTI_IRQn EXTI4_15_IRQn
#define Sensor_A_Pin GPIO_PIN_6
#define Sensor_A_GPIO_Port GPIOA
#define Sensor_A_EXTI_IRQn EXTI4_15_IRQn
#define Sensor_B_Pin GPIO_PIN_7
#define Sensor_B_GPIO_Port GPIOA
#define Sensor_B_EXTI_IRQn EXTI4_15_IRQn
#define HW_V0_Pin GPIO_PIN_0
#define HW_V0_GPIO_Port GPIOB
#define HW_V1_Pin GPIO_PIN_1
#define HW_V1_GPIO_Port GPIOB
#define ID_Pin GPIO_PIN_2
#define ID_GPIO_Port GPIOB
#define OLED_RESET_Pin GPIO_PIN_6
#define OLED_RESET_GPIO_Port GPIOC
#define OLED_SCL_Pin GPIO_PIN_11
#define OLED_SCL_GPIO_Port GPIOA
#define OLED_SDA_Pin GPIO_PIN_12
#define OLED_SDA_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define TapeDetect_Pin GPIO_PIN_15
#define TapeDetect_GPIO_Port GPIOA
#define TapeDetect_EXTI_IRQn EXTI4_15_IRQn
#define ExtDetect_Pin GPIO_PIN_3
#define ExtDetect_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOB
#define Button_C_Pin GPIO_PIN_5
#define Button_C_GPIO_Port GPIOB
#define Button_C_EXTI_IRQn EXTI4_15_IRQn
#define TX_Pin GPIO_PIN_6
#define TX_GPIO_Port GPIOB
#define RX_Pin GPIO_PIN_7
#define RX_GPIO_Port GPIOB
#define Button_A_Pin GPIO_PIN_8
#define Button_A_GPIO_Port GPIOB
#define Button_A_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
