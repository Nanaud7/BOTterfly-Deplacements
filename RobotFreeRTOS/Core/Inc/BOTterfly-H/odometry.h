/**
 ******************************************************************************
 * @file	odo.h
 * @author 	Arnaud CHOBERT
 * @brief	Odometry of BOTterfly
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_ODOMETRY_H_
#define INC_ODOMETRY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>
#include <BOTterfly-H/modules.h>
#include <BOTterfly-H/encoder.h>

/* Exported types ------------------------------------------------------------*/
/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
#define ENTRAXE 185
#define WHEEL_DIAMETER 39.5 // 40mm theorical
#define WHEEL_PERIMETER ((float)WHEEL_DIAMETER * (float)3.1415)
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void ODO_Init();
void ODO_OdometryUpdate();
double ODO_GetX();
double ODO_GetY();
double ODO_GetO();
/* End of functions ----------------------------------------------------------*/

#endif /* INC_ODOMETRY_H_ */
