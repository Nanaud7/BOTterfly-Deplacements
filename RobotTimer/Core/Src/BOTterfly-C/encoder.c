/**
 ******************************************************************************
 * @file	encoders.c
 * @author 	Arnaud CHOBERT
 * @brief	Retrieve data from an encoder (used for FIT0520's encoders)
 ******************************************************************************
 */

#include <BOTterfly-H/encoder.h>

/* Exported functions --------------------------------------------------------*/

/**
 * @defgroup Encoder initialization functions
 */

/*	@brief	Init of the ENC_HandleTypeDef structure with the timer settings
 *	@param	Encoder is a ENC_HandleTypeDef
 *	@param 	htim is a TIM handle
 *  @param  Channel_A is the TIM Channels
 *  @param  Channel_B is the TIM Channels
 *	@retval HAL_Status
 */
uint8_t ENC_InitTimer(ENC_HandleTypeDef* Encoder, TIM_HandleTypeDef *htim, uint32_t Channel_A, uint32_t Channel_B){
	Encoder->Timer = htim;
	Encoder->Timer_Channel_A = Channel_A;
	Encoder->Timer_Channel_B = Channel_B;

	if(HAL_OK != HAL_TIM_Encoder_Start(Encoder->Timer, Encoder->Timer_Channel_A & Encoder->Timer_Channel_B))
		return 1;

	return 0;
}

/*	@brief	Set the TicksPerRev value of the encoder
 *	@param	Encoder is a ENC_HandleTypeDef
 *	@param 	ticksPerRev value
 */
void ENC_SetTicksPerRev(ENC_HandleTypeDef* Encoder, float ticksPerRev){
	Encoder->TicksPerRev = ticksPerRev;
}

/**
 * @defgroup Encoder retrieving and control functions
 */

/*	@brief	Get the counter value of the timer
 *	@param	Encoder is a Enc_Struct structure
 *	@retval CNT value of the timer
 */
int16_t ENC_GetCnt(ENC_HandleTypeDef* Encoder){
	//uint16_t ticks = __HAL_TIM_GET_COUNTER(Encoder->Timer);
	int16_t ticks = Encoder->Timer->Instance->CNT;
	ENC_ResetCnt(Encoder);

	return ticks;
}

/*	@brief	Reset the counter value of the timer
 *	@param	Encoder is a Enc_Struct structure
 *	@retval 0
 */
uint8_t ENC_ResetCnt(ENC_HandleTypeDef* Encoder){
	Encoder->Timer->Instance->CNT = 0;

	return 0;
}
