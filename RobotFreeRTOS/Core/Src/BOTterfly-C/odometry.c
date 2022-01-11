/**
 ******************************************************************************
 * @file	odo.c
 * @author 	Arnaud CHOBERT
 * @brief	Odometry for BOTterfly
 ******************************************************************************
 */

#include <BOTterfly-H/odometry.h>

/* Macros --------------------------------------------------------------------*/
/* End of macros -------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
double x = 0; // Position x of the robot in mm
double y = 0; // Position y of the robot in mm
double o = 0; // Orientation of the robot in rad

/* End of variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * ODO_Init : Initialization of odometry module
 */
void ODO_Init(){
	x = 0;
	y = 0;
	o = 0;
}

/**
 * ODO_GetX : Get x coordinate of the robot
 * @return x
 */
double ODO_GetX(){
	return x;
}

/**
 * ODO_GetY : Get y coordinate of the robot
 * @return y
 */
double ODO_GetY(){
	return y;
}

/**
 * ODO_GetO : Get o angle of the robot
 * @return o
 */
double ODO_GetO(){
	return o;
}

/**
 * ODO_OdometryUpdate : Update of robot's coordinate
 * @param leftTicks	Ticks from left encoder
 * @param rightTicks Ticks from right encoder
 */
void ODO_OdometryUpdate(int32_t leftTicks, int32_t rightTicks){

	leftTicks = leftTicks * (-1);

	float dDistance = ((float)leftTicks * CodeurGauche.TicksCoeff
			+ (float)rightTicks * CodeurDroite.TicksCoeff) / 2;
	float dAngle = ((float)rightTicks * CodeurDroite.TicksCoeff
			- (float)leftTicks * CodeurGauche.TicksCoeff)
			/ (float)ENTRAXE;

	x += dDistance * cos(o);
	y += dDistance * sin(o);
	o += dAngle;

	if(o > M_PI)
		o -= M_PI * 2.0f;

	if(o < (-1) * M_PI)
		o += M_PI * 2.0f;
}

/* End of functions ----------------------------------------------------------*/
