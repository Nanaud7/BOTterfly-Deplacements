/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_Pin GPIO_PIN_13
#define BTN_GPIO_Port GPIOC
#define BTN_EXTI_IRQn EXTI15_10_IRQn
#define MOTG_IN1_Pin GPIO_PIN_0
#define MOTG_IN1_GPIO_Port GPIOC
#define MOTG_IN2_Pin GPIO_PIN_1
#define MOTG_IN2_GPIO_Port GPIOC
#define ENCG_A_Pin GPIO_PIN_0
#define ENCG_A_GPIO_Port GPIOA
#define ENCG_B_Pin GPIO_PIN_1
#define ENCG_B_GPIO_Port GPIOA
#define PH_ADC0_Pin GPIO_PIN_4
#define PH_ADC0_GPIO_Port GPIOA
#define PH_ADC1_Pin GPIO_PIN_5
#define PH_ADC1_GPIO_Port GPIOA
#define ENCD_A_Pin GPIO_PIN_6
#define ENCD_A_GPIO_Port GPIOA
#define ENCD_B_Pin GPIO_PIN_7
#define ENCD_B_GPIO_Port GPIOA
#define PH_ADC2_Pin GPIO_PIN_4
#define PH_ADC2_GPIO_Port GPIOC
#define PH_ADC3_Pin GPIO_PIN_5
#define PH_ADC3_GPIO_Port GPIOC
#define TOF_GPIOI1_Pin GPIO_PIN_0
#define TOF_GPIOI1_GPIO_Port GPIOB
#define TOF_GPIOI1_EXTI_IRQn EXTI0_IRQn
#define TOF_GPIOI2_Pin GPIO_PIN_1
#define TOF_GPIOI2_GPIO_Port GPIOB
#define TOF_GPIOI2_EXTI_IRQn EXTI1_IRQn
#define RGB_S3_Pin GPIO_PIN_12
#define RGB_S3_GPIO_Port GPIOB
#define RGB_S2_Pin GPIO_PIN_13
#define RGB_S2_GPIO_Port GPIOB
#define RGB_S1_Pin GPIO_PIN_14
#define RGB_S1_GPIO_Port GPIOB
#define RGB_S0_Pin GPIO_PIN_15
#define RGB_S0_GPIO_Port GPIOB
#define TOF_XSHUT2_Pin GPIO_PIN_6
#define TOF_XSHUT2_GPIO_Port GPIOC
#define TOF_XSHUT1_Pin GPIO_PIN_7
#define TOF_XSHUT1_GPIO_Port GPIOC
#define TOF_XSHUT0_Pin GPIO_PIN_8
#define TOF_XSHUT0_GPIO_Port GPIOC
#define RGB_TIM_Pin GPIO_PIN_9
#define RGB_TIM_GPIO_Port GPIOC
#define MOTG_PWM_Pin GPIO_PIN_8
#define MOTG_PWM_GPIO_Port GPIOA
#define MOTD_PWM_Pin GPIO_PIN_9
#define MOTD_PWM_GPIO_Port GPIOA
#define RGB_LED_Pin GPIO_PIN_11
#define RGB_LED_GPIO_Port GPIOA
#define RGB_OE_Pin GPIO_PIN_12
#define RGB_OE_GPIO_Port GPIOA
#define TOF_GPIOI0_Pin GPIO_PIN_15
#define TOF_GPIOI0_GPIO_Port GPIOA
#define TOF_GPIOI0_EXTI_IRQn EXTI15_10_IRQn
#define MOTD_IN3_Pin GPIO_PIN_8
#define MOTD_IN3_GPIO_Port GPIOB
#define MOTD_IN4_Pin GPIO_PIN_9
#define MOTD_IN4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
