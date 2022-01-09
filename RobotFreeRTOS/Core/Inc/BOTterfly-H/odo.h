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
#include <BOTterfly-H/modules.h>
#include <BOTterfly-H/encoder.h>
#include "stdint.h"
#include "math.h"

/* Exported types ------------------------------------------------------------*/
typedef struct ODO_HandleTypeDef{
	// Last encoders ticks
	int32_t leftTicks;
	int32_t rightTicks;
} ODO_HandleTypeDef;

/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
#define ENTRAXE 185
#define WHEEL_DIAMETER 39.5 // 40 mm en théorie
#define WHEEL_PERIMETER ((float)WHEEL_DIAMETER * (float)3.1415)
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern struct ODO_HandleTypeDef Odometry;
/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
uint8_t Odo_Init();
uint8_t Odo_OdometryUpdate();
double Odo_GetX();
double Odo_GetY();
double Odo_GetO();
/* End of functions ----------------------------------------------------------*/

#endif /* INC_ODOMETRY_H_ */
