/**
 ******************************************************************************
 * @file	odo.c
 * @author 	Arnaud CHOBERT
 * @brief	Odometry of BOTterfly
 ******************************************************************************
 */

#include <BOTterfly-H/odo.h>

/* Macros --------------------------------------------------------------------*/

/* End of macros -------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
struct ODO_HandleTypeDef Odometry;
double x = 0; // Position x of the robot in mm
double y = 0; // Position y of the robot in mm
double o = 0; // Orientation of the robot in rad
double leftSpeed = 0, rightSpeed = 0;

/* End of variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * Initialization of odometry module
 * @return 0
 */
uint8_t Odo_Init(){
	Odometry.leftTicks = 0;
	Odometry.rightTicks = 0;

	x = 0;
	y = 0;
	o = 0;
	return 0;
}

double Odo_GetX(){
	return x;
}

double Odo_GetY(){
	return y;
}

double Odo_GetO(){
	return o;
}

/**
 *
 * @param Odo
 * @return
 */
uint8_t Odo_OdometryUpdate(int32_t leftTicks, int32_t rightTicks){

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

	//printf("x:%.1f\t\to:%.1f\r\n", x, o);
	//printf("left:%d\t\tright:%d\r\n", Odo->leftTicks, Odo->rightTicks);

	//ENC_ResetCnt(Odo->LeftEncoder);
	//ENC_ResetCnt(Odo->RightEncoder);

	return 0;
}

/* End of functions ----------------------------------------------------------*/
