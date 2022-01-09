/**
 ******************************************************************************
 * @file	motors.c
 * @author 	Arnaud CHOBERT
 * @brief	Driving a DC motor (used for FIT0520 motors)
 ******************************************************************************
 */

#include <BOTterfly-H/motor.h>

/* Exported functions --------------------------------------------------------*/

/**
 * @defgroup Motor initialization functions
 */

/**	@brief	Init of the MOT_HandleTypeDef structure with the timer settings
 *	@param	Motor is a MOT_HandleTypeDef
 *	@param 	htim is a TIM handle
 *  @param  Channel is the TIM Channel used
 *	@retval HAL status
 */
uint8_t MOT_InitTimer(MOT_HandleTypeDef* Motor, TIM_HandleTypeDef *htim, uint32_t Channel){
	Motor->Timer_Handle = htim;
	Motor->Timer_Channel = Channel;

	if(HAL_OK != HAL_TIM_PWM_Start(Motor->Timer_Handle, Motor->Timer_Channel))
		return 1;

	return 0;
}

/**	@brief	Init of the MOT_HandleTypeDef structure with the GPIOs settings
 *	@param	Motor is a MOT_HandleTypeDef
 *	@param 	IN1_GPIOx is a GPIOx where x can be (A..K) to select the GPIO peripheral
 *  @param  IN1_GPIO_Pin specifies the port bit
 *	@param 	IN2_GPIOx is a GPIOx where x can be (A..K) to select the GPIO peripheral
 *  @param  IN2_GPIO_Pin specifies the port bit
 */
void MOT_InitGPIOs(MOT_HandleTypeDef* Motor, GPIO_TypeDef* IN1_GPIOx, uint16_t IN1_GPIO_Pin,
		GPIO_TypeDef* IN2_GPIOx, uint16_t IN2_GPIO_Pin){
	Motor->IN1_GPIOx = IN1_GPIOx;
	Motor->IN1_GPIO_Pin = IN1_GPIO_Pin;
	Motor->IN2_GPIOx = IN2_GPIOx;
	Motor->IN2_GPIO_Pin = IN2_GPIO_Pin;
}


/**	@brief	Set coefficient for the error correction of the motor
 *	@param	Motor is a MOT_HandleTypeDef
 *	@param 	Kp coefficient
 *	@param 	Ki coefficient
 */
void MOT_SetCoeff(MOT_HandleTypeDef* Motor, float Kp, float Ki){
	Motor->Kp = Kp;
	Motor->Ki = Ki;
}


/**
 * @defgroup Motor initialization functions
 */

/*	@brief	Set the direction of the motor
 *	@param	Motor is a MOT_HandleTypeDef
 *	@param	direction specifies the driver function applied to the motor
 *		@arg	MOT_FUNCTIONS_STOP
 *		@arg 	MOT_FUNCTIONS_FORWARD
 *		@arg 	MOT_FUNCTIONS_REVERSE
 */

void MOT_SetDirection(MOT_HandleTypeDef* Motor, uint8_t direction){
	switch(direction){

	case MOT_FUNCTIONS_STOP:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_RESET);
		break;

	case MOT_FUNCTIONS_FORWARD:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_SET);
		break;

	case MOT_FUNCTIONS_REVERSE:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_RESET);
		break;

	default:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_RESET);
	}
}

/*	@brief	Set the duty cycle of the PWM signal
 *	@param	Motor is a MOT_HandleTypeDef
 *	@param	duty_cycle is value between 0 and 100
 */
void MOT_SetDutyCycle(MOT_HandleTypeDef* Motor, float duty_cycle){
	if(duty_cycle > 80) duty_cycle = 80;
	else if(duty_cycle < 0) duty_cycle = 0;

	float pulse = (duty_cycle / 100) * (float)MOT_TIM_PERIOD;

	__HAL_TIM_SET_COMPARE(Motor->Timer_Handle, Motor->Timer_Channel, (uint32_t)pulse);
}

/* End of exported functions -------------------------------------------------*/
