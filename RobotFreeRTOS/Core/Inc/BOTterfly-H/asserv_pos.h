/**
 ******************************************************************************
 * @file	asserv_pos.h
 * @author 	Arnaud CHOBERT
 * @brief	Asservissement en position de BOTterfly
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BOTTERFLY_H_ASSERV_POS_H_
#define INC_BOTTERFLY_H_ASSERV_POS_H_

/* Includes ------------------------------------------------------------------*/
#include <BOTterfly-H/odometry.h>
#include <stdio.h>
#include <math.h>
#include "BOTterfly-H/modules.h"
#include "BOTterfly-H/asserv_vit.h"
#include "BOTterfly-H/shellOS.h"

/* Exported types ------------------------------------------------------------*/
/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void Pos_ControlLoop_2steps();
void setTargetX(double x);
void setTargetY(double y);
uint8_t isArrived();
/* End of exported functions -------------------------------------------------*/

#endif /* INC_BOTTERFLY_H_ASSERV_POS_H_ */
