/**
 ******************************************************************************
 * @file	encoder.h
 * @author 	Arnaud CHOBERT
 * @brief	Retrieve data from an encoder (used for FIT0521 motors)
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_ENCODERS_H_
#define INC_ENCODERS_H_

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* Exported types ------------------------------------------------------------*/

// Encoder handle structure
typedef struct ENC_HandleTypeDef{
	TIM_HandleTypeDef* Timer;	/* Timer Handle			*/
	uint32_t Timer_Channel_A;	/* Timer Channel		*/
	uint32_t Timer_Channel_B;	/* Timer Channel		*/

	double TicksCoeff;
} ENC_HandleTypeDef;

/* End of exported types -----------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define ENC_TICKS_PER_REV 900
#define ENC_DIST_PER_TICK ((float)WHEEL_PERIMETER / (float)ENC_TICKS_PER_REV)

/* End of exported constants -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

uint8_t ENC_InitTimer(ENC_HandleTypeDef* Encoder, TIM_HandleTypeDef *htim, uint32_t Channel_A, uint32_t Channel_B);
void ENC_SetTicksPerRev(ENC_HandleTypeDef* Encoder, float ticksPerRev);
void ENC_SetMillimeterPerTick(ENC_HandleTypeDef* Encoder, double millimeterPerTick);
int32_t ENC_GetCnt(ENC_HandleTypeDef* Encoder);
void ENC_ResetCnt(ENC_HandleTypeDef* Encoder);

/* End of exported functions -------------------------------------------------*/

#endif /* INC_ENCODERS_H_ */
