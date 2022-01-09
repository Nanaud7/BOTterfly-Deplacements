/**
 ******************************************************************************
 * @file	motor.h
 * @author 	Arnaud CHOBERT
 * @brief	Driving a DC motor (used for FIT0520 motors)
 ******************************************************************************
 */

/**
 ******************************************************************************
 * @tutorial for one motor
 *	- 	Configure a Timer with the PWM generation mode.
 *		Set the frequency to 25kHz and set the duty cycle to 0%.
 *		Ex: Clk = 84MHz, PSC = 0, ARR = 3360-1 and Pulse = 0;
 *	-	Configure two GPIOs
 *	- 	Configure one ADC with interrupts, with Continuous Conversion Mode and
 *		with ADC_EOC_SEQ_CONV
 *
 *	@exemple
 *	Mot_Struct MoteurA;
 * Mot_Init_SetTimer(&Motor, &htim1, TIM_CHANNEL_1);
 * Mot_Init_SetGPIOs(&Motor, GPIOC, GPIO_PIN_0, GPIOC, GPIO_PIN_1);
 * Mot_SetDirection(&Motor, MOTOR_FORWARD);
 * Mot_SetDutyCycle(&Motor, 65);
 *
 * void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
 * 		if(hadc->Instance == ADC1){
 * 		uint32_t val = HAL_ADC_GetValue(&hadc1);
 * }
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief	MOT Handle structure definition
 */
typedef struct MOT_HandleTypeDef{
	TIM_HandleTypeDef* Timer_Handle;	/* Timer Handle					*/
	uint32_t Timer_Channel;				/* Timer Channel				*/

	GPIO_TypeDef* IN1_GPIOx;			/* GPIO Port for IN1			*/
	uint16_t IN1_GPIO_Pin;				/* GPIO Pin for IN1				*/
	GPIO_TypeDef* IN2_GPIOx;			/* GPIO Port for IN2			*/
	uint16_t IN2_GPIO_Pin;				/* GPIO Pin for IN2				*/

	ADC_HandleTypeDef* ADC_Handle;		/* ADC Handle					*/

	float Kp;							/* Coeff Kp error correction	*/
	float Ki;							/* Coeff Ki error correction	*/
	double s_previous;					/* Commande precedente 			*/
	double err_previous;				/* Erreur n-1	 				*/
} MOT_HandleTypeDef;

/* End of exported types -----------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/**
 * @defgroup L298N DRIVER FUNCTIONS
 */
#define MOT_FUNCTIONS_STOP 		0
#define MOT_FUNCTIONS_FORWARD 	1
#define MOT_FUNCTIONS_REVERSE 	2

#define MOT_TIM_PERIOD 3360 // TIMER CONSTANTS
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
