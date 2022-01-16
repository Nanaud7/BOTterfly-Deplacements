/**
 ******************************************************************************
 * @file	motors.c
 * @author 	Arnaud CHOBERT
 * @brief	Driving a DC motor (used for FIT0521 motors)
 ******************************************************************************
 */

#include <BOTterfly-H/Motors/motor.h>

/* Exported functions --------------------------------------------------------*/

/**	MOT_InitTimer : Initialization of MOT_HandleTypeDef structure
 *	@param	Motor MOT_HandleTypeDef
 *	@param 	htim TIM_HandleTypeDef
 *  @param  Channel TIM Channel used
 *	@retval HAL status
 */
uint8_t MOT_InitTimer(MOT_HandleTypeDef* Motor, TIM_HandleTypeDef *htim, uint32_t Channel){
	Motor->Timer_Handle = htim;
	Motor->Timer_Channel = Channel;

	if(HAL_OK != HAL_TIM_PWM_Start(Motor->Timer_Handle, Motor->Timer_Channel))
		return 1;

	return 0;
}

/**	MOT_InitGPIOs :	Initialization of MOT_HandleTypeDef structure
 *	@param	Motor MOT_HandleTypeDef
 *	@param 	IN1_GPIOx GPIO port
 *  @param  IN1_GPIO_Pin GPIO pin
 *	@param 	IN2_GPIOx GPIO port
 *  @param  IN2_GPIO_Pin GPIO pin
 */
void MOT_InitGPIOs(MOT_HandleTypeDef* Motor, GPIO_TypeDef* IN1_GPIOx, uint16_t IN1_GPIO_Pin,
		GPIO_TypeDef* IN2_GPIOx, uint16_t IN2_GPIO_Pin){
	Motor->IN1_GPIOx = IN1_GPIOx;
	Motor->IN1_GPIO_Pin = IN1_GPIO_Pin;
	Motor->IN2_GPIOx = IN2_GPIOx;
	Motor->IN2_GPIO_Pin = IN2_GPIO_Pin;
}


/**	MOT_SetCoeff : Set coefficient for the error correction of the motor
 *	@param	Motor MOT_HandleTypeDef
 *	@param 	Kp coefficient
 *	@param 	Ki coefficient
 */
void MOT_SetCoeff(MOT_HandleTypeDef* Motor, float Kp, float Ki){
	Motor->Kp = Kp;
	Motor->Ki = Ki;
}


/**
 * MOT_SetDirection : Set the direction of the motor
 * @param Motor MOT_HandleTypeDef
 * @param direction
 */
void MOT_SetDirection(MOT_HandleTypeDef* Motor, uint8_t direction){
	switch(direction){

	case MOT_FUNCTIONS_STOP:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_RESET);
		break;

	case MOT_FUNCTIONS_REVERSE:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_SET);
		break;

	case MOT_FUNCTIONS_FORWARD:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_RESET);
		break;

	default:
		HAL_GPIO_WritePin(Motor->IN1_GPIOx, Motor->IN1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Motor->IN2_GPIOx, Motor->IN2_GPIO_Pin, GPIO_PIN_RESET);
	}
}

/**
 * MOT_SetDutyCycle : Set the duty cycle of the PWM signal
 * @param Motor MOT_HandleTypeDef
 * @param duty_cycle Value between 0 and 100
 */
void MOT_SetDutyCycle(MOT_HandleTypeDef* Motor, float duty_cycle){
	if(duty_cycle > 80) duty_cycle = 80;
	else if(duty_cycle < 0) duty_cycle = 0;

	float pulse = (duty_cycle / 100) * (float)MOT_TIM_PERIOD;

	__HAL_TIM_SET_COMPARE(Motor->Timer_Handle, Motor->Timer_Channel, (uint32_t)pulse);
}

/* End of exported functions -------------------------------------------------*/
