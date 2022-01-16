/**
 ******************************************************************************
 * @file	asserv_vit.h
 * @author 	Arnaud CHOBERT
 * @brief	Speed loop control for BOTterfly
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

/* Includes ------------------------------------------------------------------*/
#include <BOTterfly-H/Motors/encoder.h>
#include <BOTterfly-H/Motors/modules.h>
#include <BOTterfly-H/Motors/motor.h>
#include <BOTterfly-H/Motors/shellOS.h>
#include <math.h>
#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
int32_t CTRL_SpeedControl(MOT_HandleTypeDef* Motor, ENC_HandleTypeDef* Encoder,
		int32_t ticks, double consigne);

/* End of exported functions -------------------------------------------------*/

#endif /* INC_CONTROL_H_ */
