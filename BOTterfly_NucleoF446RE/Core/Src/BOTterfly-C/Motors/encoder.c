/**
 ******************************************************************************
 * @file	encoders.c
 * @author 	Arnaud CHOBERT
 * @brief	Retrieve data from an encoder (used for FIT0521's encoders)
 ******************************************************************************
 */

#include <BOTterfly-H/Motors/encoder.h>

/* Exported functions --------------------------------------------------------*/

/**
 * ENC_InitTimer : Initialization of ENC_HandleTypeDef structure
 * @param Encoder ENC_HandleTypeDef
 * @param htim TIM_HandleTypeDef
 * @param Channel_A TIM Channel used
 * @param Channel_B TIM Channel used
 * @return HAL_Status
 */
uint8_t ENC_InitTimer(ENC_HandleTypeDef* Encoder, TIM_HandleTypeDef *htim, uint32_t Channel_A, uint32_t Channel_B){
	Encoder->Timer = htim;
	Encoder->Timer_Channel_A = Channel_A;
	Encoder->Timer_Channel_B = Channel_B;

	if(HAL_OK != HAL_TIM_Encoder_Start(Encoder->Timer, Encoder->Timer_Channel_A & Encoder->Timer_Channel_B))
		return 1;

	return 0;
}


/**
 * ENC_SetMillimeterPerTick : Set MillimeterPerTick coefficient
 * @param Encoder ENC_HandleTypeDef
 * @param millimeterPerTick
 */
void ENC_SetMillimeterPerTick(ENC_HandleTypeDef* Encoder, double millimeterPerTick){
	Encoder->TicksCoeff = millimeterPerTick;
}


/**
 * ENC_GetCnt : Get CNT value of the timer counter
 * @param Encoder ENC_HandleTypeDef
 * @return ticks CNT value
 */
int16_t ENC_GetCnt(ENC_HandleTypeDef* Encoder){
	int32_t ticks = __HAL_TIM_GET_COUNTER(Encoder->Timer);
	//int32_t ticks = Encoder->Timer->Instance->CNT;
	ENC_ResetCnt(Encoder);

	return ticks;
}

/**
 * ENC_ResetCnt : Reset CNT register of the timer counter
 * @param Encoder ENC_HandleTypeDef
 */
void ENC_ResetCnt(ENC_HandleTypeDef* Encoder){
	Encoder->Timer->Instance->CNT = 0;
}
