/**
 ******************************************************************************
 * @file	ctrl_positionV1.h
 * @author 	Arnaud CHOBERT
 * @brief	Position control of BOTterfly V1
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BOTTERFLY_H_CTRL_POSITIONV1_H_
#define INC_BOTTERFLY_H_CTRL_POSITIONV1_H_

/* Includes ------------------------------------------------------------------*/
#include <BOTterfly-H/ctrl_speed.h>
#include <BOTterfly-H/odometry.h>
#include <stdio.h>
#include <math.h>
#include "BOTterfly-H/modules.h"
#include "BOTterfly-H/shellOS.h"

/* Exported types ------------------------------------------------------------*/
/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void CTRL_Pos1_PositionControl();
void CTRL_Pos1_SetTargetX(double x);
void CTRL_Pos1_SetTargetY(double y);
uint8_t CTRL_Pos1_isArrived();
/* End of exported functions -------------------------------------------------*/

#endif /* INC_BOTTERFLY_H_CTRL_POSITIONV1_H_ */
