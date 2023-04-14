/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32mp1xx_hal.h"

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
#define RS232_TX_Pin GPIO_PIN_5
#define RS232_TX_GPIO_Port GPIOD
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define PWM3_Pin GPIO_PIN_9
#define PWM3_GPIO_Port GPIOB
#define PWM7_Pin GPIO_PIN_0
#define PWM7_GPIO_Port GPIOI
#define PWM6_Pin GPIO_PIN_11
#define PWM6_GPIO_Port GPIOH
#define PWM2_Pin GPIO_PIN_9
#define PWM2_GPIO_Port GPIOH
#define RS232_RX_Pin GPIO_PIN_6
#define RS232_RX_GPIO_Port GPIOD
#define PWM4_Pin GPIO_PIN_7
#define PWM4_GPIO_Port GPIOB
#define RS485_RX_Pin GPIO_PIN_9
#define RS485_RX_GPIO_Port GPIOD
#define RS485_TX_Pin GPIO_PIN_8
#define RS485_TX_GPIO_Port GPIOD
#define PWM8_Pin GPIO_PIN_3
#define PWM8_GPIO_Port GPIOA
#define PWM1_Pin GPIO_PIN_10
#define PWM1_GPIO_Port GPIOB
#define CAN2_RX_Pin GPIO_PIN_5
#define CAN2_RX_GPIO_Port GPIOB
#define CAN2_TX_Pin GPIO_PIN_6
#define CAN2_TX_GPIO_Port GPIOB
#define RS485_DE_Pin GPIO_PIN_12
#define RS485_DE_GPIO_Port GPIOD
#define PWM5_Pin GPIO_PIN_11
#define PWM5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define AD7441x_ALERT_Pin GPIO_PIN_10
#define AD7441x_ALERT_GPIO_Port GPIOA
#define AD7441x_RDY_Pin GPIO_PIN_13
#define AD7441x_RDY_GPIO_Port GPIOD
#define AD7441x_RESET_Pin GPIO_PIN_2
#define AD7441x_RESET_GPIO_Port GPIOB
#define AD7441x_CS_Pin GPIO_PIN_15
#define AD7441x_CS_GPIO_Port GPIOD
#define AD7441x_LDAC_Pin GPIO_PIN_10
#define AD7441x_LDAC_GPIO_Port GPIOE

#define DO0_Pin GPIO_PIN_8
#define DO0_GPIO_Port GPIOI
#define DO1_Pin GPIO_PIN_15
#define DO1_GPIO_Port GPIOF
#define DO2_Pin GPIO_PIN_7
#define DO2_GPIO_Port GPIOF
#define DO3_Pin GPIO_PIN_11
#define DO3_GPIO_Port GPIOD
#define DO4_Pin GPIO_PIN_0
#define DO4_GPIO_Port GPIOG
#define DO5_Pin GPIO_PIN_14
#define DO5_GPIO_Port GPIOA
#define DO6_Pin GPIO_PIN_5
#define DO6_GPIO_Port GPIOA
#define DO7_Pin GPIO_PIN_3
#define DO7_GPIO_Port GPIOF

#define DI1_Pin GPIO_PIN_14
#define DI1_GPIO_Port GPIOE
#define DI2_Pin GPIO_PIN_11
#define DI2_GPIO_Port GPIOE
#define DI3_Pin GPIO_PIN_13
#define DI3_GPIO_Port GPIOE
#define DI4_Pin GPIO_PIN_10
#define DI4_GPIO_Port GPIOH
#define DI5_Pin GPIO_PIN_2
#define DI5_GPIO_Port GPIOI
#define DI6_Pin GPIO_PIN_5
#define DI6_GPIO_Port GPIOI
#define DI7_Pin GPIO_PIN_7
#define DI7_GPIO_Port GPIOI
#define DI8_Pin GPIO_PIN_6
#define DI8_GPIO_Port GPIOI
#define DI9_Pin GPIO_PIN_5
#define DI9_GPIO_Port GPIOE
#define DI10_Pin GPIO_PIN_6
#define DI10_GPIO_Port GPIOC
#define DI11_Pin GPIO_PIN_8
#define DI11_GPIO_Port GPIOA
#define DI12_Pin GPIO_PIN_7
#define DI12_GPIO_Port GPIOC

#define SYS_LED_Pin GPIO_PIN_11
#define SYS_LED_GPIO_Port GPIOA

#define PWMG0_CS_DIS_Pin GPIO_PIN_10
#define PWMG0_CS_DIS_GPIO_Port GPIOF
#define PWMG1_CS_DIS_Pin GPIO_PIN_9
#define PWMG1_CS_DIS_GPIO_Port GPIOE

// #define PWM1_CS_Pin
// #define PWM1_CS_GPIO_Port
// #define PWM2_CS_Pin
// #define PWM2_CS_GPIO_Port
// #define PWM3_CS_Pin
// #define PWM3_CS_GPIO_Port
// #define PWM4_CS_Pin
// #define PWM4_CS_GPIO_Port
// #define PWM5_CS_Pin
// #define PWM5_CS_GPIO_Port
// #define PWM6_CS_Pin
// #define PWM6_CS_GPIO_Port
// #define PWM7_CS_Pin
// #define PWM7_CS_GPIO_Port
// #define PWM8_CS_Pin
// #define PWM8_CS_GPIO_Port

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
