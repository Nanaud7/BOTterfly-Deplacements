/**
 ******************************************************************************
 * @file	encoder.h
 * @author 	Arnaud CHOBERT
 * @brief	Driving a DC motor (used for FIT0520 motors)
 ******************************************************************************
 */

/**
 ******************************************************************************
 * @tutorial for one encoder
 *	- 	Configure a Timer with the Encoder Mode
 *	- ...
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_ENCODERS_H_
#define INC_ENCODERS_H_

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief	ENC Handle structure definition
 */
typedef struct ENC_HandleTypeDef{
	TIM_HandleTypeDef* Timer;	/* Timer Handle			*/
	uint32_t Timer_Channel_A;	/* Timer Channel		*/
	uint32_t Timer_Channel_B;	/* Timer Channel		*/

	uint16_t TicksPerRev;		/* Ticks per revolution	*/
	float TicksCoeff;
} ENC_HandleTypeDef;

/* End of exported types -----------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/**
 * @defgroup WHEEL CONSTANTS
 */



/**
 * @defgroup ENCODER CONSTANTS
 */
#define ENC_TICKS_PER_REV 900
#define ENC_DIST_PER_TICK ((float)WHEEL_PERIMETER / (float)ENC_TICKS_PER_REV)

/* End of exported constants -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Encoder initialization functions
 */
uint8_t ENC_InitTimer(ENC_HandleTypeDef* Encoder, TIM_HandleTypeDef *htim, uint32_t Channel_A, uint32_t Channel_B);
void ENC_SetTicksPerRev(ENC_HandleTypeDef* Encoder, float ticksPerRev);

/**
 * @brief Encoder retrieving and control functions
 */
int16_t ENC_GetCnt(ENC_HandleTypeDef* Encoder);
uint8_t ENC_ResetCnt(ENC_HandleTypeDef* Encoder);

/* End of exported functions -------------------------------------------------*/

#endif /* INC_ENCODERS_H_ */
