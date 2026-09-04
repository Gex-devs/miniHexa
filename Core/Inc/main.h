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
#include "stm32f4xx_hal.h"

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
void MX_SPI1_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define UART2_LOG_TX_Pin GPIO_PIN_2
#define UART2_LOG_TX_GPIO_Port GPIOA
#define UART2_LOG_RX_Pin GPIO_PIN_3
#define UART2_LOG_RX_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_12
#define SPI_CS_GPIO_Port GPIOB
#define SPI_RDY_Pin GPIO_PIN_8
#define SPI_RDY_GPIO_Port GPIOC
#define IMU_INT_Pin GPIO_PIN_9
#define IMU_INT_GPIO_Port GPIOC
#define IMU_INT_EXTI_IRQn EXTI9_5_IRQn
#define CHIP_EN_Pin GPIO_PIN_8
#define CHIP_EN_GPIO_Port GPIOA
#define USER_BUTTON_Pin GPIO_PIN_11
#define USER_BUTTON_GPIO_Port GPIOA
#define SERVO_TX_Pin GPIO_PIN_6
#define SERVO_TX_GPIO_Port GPIOB
#define SERVO_RX_Pin GPIO_PIN_7
#define SERVO_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
