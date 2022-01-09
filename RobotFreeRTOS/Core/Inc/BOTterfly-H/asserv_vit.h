/**
 ******************************************************************************
 * @file	asserv_vit.h
 * @author 	Arnaud CHOBERT
 * @brief	Asservissement en vitesse de BOTterfly
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

/* Includes ------------------------------------------------------------------*/
#include <BOTterfly-H/encoder.h>
#include <BOTterfly-H/modules.h>
#include <BOTterfly-H/motor.h>
#include <BOTterfly-H/shellOS.h>
#include <math.h>
#include "tim.h"

/* Exported types ------------------------------------------------------------*/
typedef struct ASSERV_VIT_HandleTypeDef{
	// Timer
	TIM_HandleTypeDef* Timer;
	// Odometry
	struct Odo_Struct *Odo;
	// Motors
	struct Mot_Struct *MotorLeft;
	struct Mot_Struct *MotorRight;
} ASSERV_VIT_HandleTypeDef;

/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern double acc;
/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
uint8_t vit_SetTimer(TIM_HandleTypeDef *htim);
uint8_t vit_SetConsigne(double new_Consigne);
int32_t vit_SpeedControl(MOT_HandleTypeDef* Motor, ENC_HandleTypeDef* Encoder,
		int32_t ticks, double consigne);

/* End of exported functions -------------------------------------------------*/

#endif /* INC_CONTROL_H_ */
