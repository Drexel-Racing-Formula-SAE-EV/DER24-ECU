/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f7xx_hal.h"

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
void MX_GPIO_Init(void);
void MX_CAN1_Init(void);
void MX_ADC1_Init(void);
void MX_ADC3_Init(void);
void MX_SPI6_Init(void);
void MX_TIM5_Init(void);
void MX_USART3_UART_Init(void);
void MX_UART7_Init(void);
void MX_RTC_Init(void);
void MX_ADC2_Init(void);
void MX_I2C2_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RTD_Go_Pin GPIO_PIN_4
#define RTD_Go_GPIO_Port GPIOE
#define BSE2_Pin GPIO_PIN_3
#define BSE2_GPIO_Port GPIOF
#define CoolTemp2_Pin GPIO_PIN_4
#define CoolTemp2_GPIO_Port GPIOF
#define CoolTemp1_Pin GPIO_PIN_5
#define CoolTemp1_GPIO_Port GPIOF
#define CoolPress_Pin GPIO_PIN_9
#define CoolPress_GPIO_Port GPIOF
#define MTR_EN_Pin GPIO_PIN_10
#define MTR_EN_GPIO_Port GPIOF
#define APPS2_Pin GPIO_PIN_0
#define APPS2_GPIO_Port GPIOC
#define BSE1_Pin GPIO_PIN_3
#define BSE1_GPIO_Port GPIOC
#define CoolFlow_Pin GPIO_PIN_0
#define CoolFlow_GPIO_Port GPIOA
#define APPS1_Pin GPIO_PIN_3
#define APPS1_GPIO_Port GPIOA
#define Firmware_Ok_Pin GPIO_PIN_7
#define Firmware_Ok_GPIO_Port GPIOA
#define TSAL_HV_SIG_Pin GPIO_PIN_5
#define TSAL_HV_SIG_GPIO_Port GPIOC
#define MTR_Fault_Pin GPIO_PIN_2
#define MTR_Fault_GPIO_Port GPIOB
#define MTR_Ok_Pin GPIO_PIN_12
#define MTR_Ok_GPIO_Port GPIOF
#define Buzzer_Pin GPIO_PIN_13
#define Buzzer_GPIO_Port GPIOF
#define IMD_Fail_Pin GPIO_PIN_14
#define IMD_Fail_GPIO_Port GPIOF
#define BMS_Fail_Pin GPIO_PIN_15
#define BMS_Fail_GPIO_Port GPIOF
#define BSPD_Fail_Pin GPIO_PIN_13
#define BSPD_Fail_GPIO_Port GPIOE
#define Brake_Light_Pin GPIO_PIN_14
#define Brake_Light_GPIO_Port GPIOD
#define SPI6_NSS_Pin GPIO_PIN_8
#define SPI6_NSS_GPIO_Port GPIOG
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
