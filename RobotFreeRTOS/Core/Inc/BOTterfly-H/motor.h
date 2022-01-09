/**
 ******************************************************************************
 * @file	motor.h
 * @author 	Arnaud CHOBERT
 * @brief	Driving a DC motor (used for FIT0521 motors)
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* Exported types ------------------------------------------------------------*/

// Motor Handle structure
typedef struct MOT_HandleTypeDef{
	TIM_HandleTypeDef* Timer_Handle;	/* Timer HandleTypeDef			*/
	uint32_t Timer_Channel;				/* Timer Channel				*/

	GPIO_TypeDef* IN1_GPIOx;			/* GPIO Port for IN1			*/
	uint16_t IN1_GPIO_Pin;				/* GPIO Pin for IN1				*/
	GPIO_TypeDef* IN2_GPIOx;			/* GPIO Port for IN2			*/
	uint16_t IN2_GPIO_Pin;				/* GPIO Pin for IN2				*/

	float Kp;							/* Coeff Kp error correction	*/
	float Ki;							/* Coeff Ki error correction	*/
	double s_previous;					/* Previous result  			*/
	double err_previous;				/* Previous error 				*/
} MOT_HandleTypeDef;

/* End of exported types -----------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

// L298N DRIVER FUNCTIONS
#define MOT_FUNCTIONS_STOP 		0
#define MOT_FUNCTIONS_FORWARD 	1
#define MOT_FUNCTIONS_REVERSE 	2

// TIMER CONSTANTS
#define MOT_TIM_PERIOD 3360
#define MOV_MIN_SPEED 40
#define MOV_MAX_SPEED 100
#define ROT_MIN_SPEED 40
#define ROT_MAX_SPEED 80

/* End of exported constants -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Motor initialization functions
 */
uint8_t MOT_InitTimer(MOT_HandleTypeDef* Motor, TIM_HandleTypeDef *htim, uint32_t Channel);
void MOT_InitGPIOs(MOT_HandleTypeDef* Motor, GPIO_TypeDef* IN1_GPIOx, uint16_t IN1_GPIO_Pin,
		GPIO_TypeDef* IN2_GPIOx, uint16_t IN2_GPIO_Pin);
void MOT_SetCoeff(MOT_HandleTypeDef* Motor, float Kp, float Ki);

/**
 * @brief Motor driving and control functions
 */
void MOT_SetDirection(MOT_HandleTypeDef* Motor, uint8_t direction);
void MOT_SetDutyCycle(MOT_HandleTypeDef* Motor, float duty_cycle);

/* End of exported functions -------------------------------------------------*/

#endif /* INC_MOTORS_H_ */
