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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <bB101_VCO.h>

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
#define RVAR8_Pin GPIO_PIN_0
#define RVAR8_GPIO_Port GPIOF
#define J6_Pin GPIO_PIN_1
#define J6_GPIO_Port GPIOF
#define TACT0_Pin GPIO_PIN_10
#define TACT0_GPIO_Port GPIOG
#define RVAR5_Pin GPIO_PIN_0
#define RVAR5_GPIO_Port GPIOA
#define J3_Pin GPIO_PIN_1
#define J3_GPIO_Port GPIOA
#define J1_Pin GPIO_PIN_2
#define J1_GPIO_Port GPIOA
#define J8_Pin GPIO_PIN_3
#define J8_GPIO_Port GPIOA
#define RVAR2_Pin GPIO_PIN_6
#define RVAR2_GPIO_Port GPIOA
#define RVAR4_Pin GPIO_PIN_7
#define RVAR4_GPIO_Port GPIOA
#define FLAG_Pin GPIO_PIN_0
#define FLAG_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_8
#define LCD_RESET_GPIO_Port GPIOA
#define TACT2_Pin GPIO_PIN_9
#define TACT2_GPIO_Port GPIOA
#define TACT1_Pin GPIO_PIN_10
#define TACT1_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_15
#define LCD_CS_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_4
#define LCD_DC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
