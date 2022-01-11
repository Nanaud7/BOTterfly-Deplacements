/**
 ******************************************************************************
 * @file	ctrl_positionV1.c
 * @author 	Arnaud CHOBERT
 * @brief	Position control of BOTterfly V1
 ******************************************************************************
 */

#include "BOTterfly-H/ctrl_positionV1.h"

/* Types ---------------------------------------------------------------------*/
/* End of types --------------------------------------------------------------*/

/* Macros --------------------------------------------------------------------*/
// Speed minimums and maximums
#define POS_LIN_SMAX 150
#define POS_LIN_SMIN 50
#define POS_ROT_SMAX 120
#define POS_ROT_SMIN 80

// States of the machine state
#define POS_STATE_STANDBY 0
#define POS_STATE_ROTATION 1
#define POS_STATE_MOVE 2

/* End of macros -------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
uint8_t state = POS_STATE_STANDBY;
double targetX = 0, targetY = 0;

double distanceToTargetInitial = 0;
double angleRelativeInitial = 0;

int32_t tckG = 0, tckD = 0; // Ticks measured (tests)

/* End of variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * setTargetX : Set new x target coordinate
 * @param x x coordinate
 */
void CTRL_Pos1_SetTargetX(double x){
	targetX = x;
	printf("setTargetX done with %lf\r\n",targetX);
}

/**
 * setTargetY : Set new y target coordinate
 * @param y y coordinate
 */
void CTRL_Pos1_SetTargetY(double y){
	targetY = y;
	printf("setTargetY done with %lf\r\n",targetY);
}

/**
 * isArrived : Check if the robot is arrvied to its target
 * @return 1 if BOTterfly arrived to its target
 */
uint8_t CTRL_Pos1_isArrived(){
	if(targetX == 0 && targetY == 0){
		return 1;
	} else{
		return 0;
	}
}

/**
 * toZeroOne
 * @param x Value needs to be scaled between 0 and 1
 * @return x Corrected value
 */
double toZeroOne(double x) {
	if (x < 0) return 0;
	if (x > 1) return 1;
	return x;
}

/**
 * toZeroTwoPi
 * @param angle Angle needs to be scaled between 0 and 2*Pi
 * @return Corrected angle
 */
double toZeroTwoPi(double angle) {
	return fmod(fmod(angle, M_TWOPI) + M_TWOPI, M_TWOPI);
}

/**
 * speedCurve
 * @param x value scaled between 0 and 1
 * @param a curve coefficient, increase this value to up the acceleration
 * @return speed calculated
 */
double speedCurve(double x, double a){
	double speed = atan(-x*(x-1)*a) / atan(0.25*a);
	return speed;
}


/**
 * Pos_ControlLoop_2steps
 */
void CTRL_Pos1_PositionControl(){
	static double spin_previous = 0;

	/* ODOMETRY ------------------------*/
	int32_t ticksLeft = ENC_GetCnt(&CodeurGauche);
	int32_t ticksRight = ENC_GetCnt(&CodeurDroite);
	ODO_OdometryUpdate(ticksLeft, ticksRight);

	/* ROTATION ------------------------*/
	double angleToTarget = atan2(targetY - ODO_GetY(), targetX - ODO_GetX());
	// Rotation direction
	double angleRelative = toZeroTwoPi(angleToTarget) - toZeroTwoPi(ODO_GetO());
	if(angleRelative > M_PI) angleRelative = angleRelative - M_TWOPI;
	else if(angleRelative < (-M_PI)) angleRelative = angleRelative + M_TWOPI;
	if (angleRelativeInitial == 0) angleRelativeInitial = angleRelative;
	// Progression
	double rotationProgress = 1 - (fabs(angleRelative) / fabs(angleRelativeInitial));
	rotationProgress = toZeroOne(rotationProgress);

	/* MOVE ----------------------------*/
	double distanceToTarget = sqrt(pow(targetX - ODO_GetX(),2) + pow(targetY - ODO_GetY(),2));
	if (distanceToTargetInitial == 0) distanceToTargetInitial = distanceToTarget;
	// Progression
	double moveProgress = 1 - toZeroOne(distanceToTarget / distanceToTargetInitial);

	// Tests
	tckG += ticksLeft;
	tckD += ticksRight;

	/* STATE MACHINE -------------------*/
	switch(state){

	case POS_STATE_STANDBY:
		MOT_SetDutyCycle(&MoteurGauche, 0);
		MOT_SetDutyCycle(&MoteurDroite, 0);

		distanceToTargetInitial = 0;
		angleRelativeInitial = 0;

		if (!CTRL_Pos1_isArrived()) {
			state = POS_STATE_ROTATION;
		}
		break;

	case POS_STATE_ROTATION:
		//printf("angleRelative = %lf\r\n", angleRelative);

		// Choix du sens de rotation
		if (angleRelative < M_PI && angleRelative > 0){
			MOT_SetDirection(&MoteurGauche, MOT_FUNCTIONS_REVERSE);
			MOT_SetDirection(&MoteurDroite, MOT_FUNCTIONS_FORWARD);
		} else {
			MOT_SetDirection(&MoteurGauche, MOT_FUNCTIONS_FORWARD);
			MOT_SetDirection(&MoteurDroite, MOT_FUNCTIONS_REVERSE);
		}

		// Contrôle de la vitesse
		double spin = POS_ROT_SMAX * speedCurve(rotationProgress, 1);
		spin += POS_ROT_SMIN * (1 - rotationProgress);
		CTRL_SpeedControl(&MoteurGauche, &CodeurGauche, ticksLeft, spin);
		CTRL_SpeedControl(&MoteurDroite, &CodeurDroite, ticksRight, spin);

		// Stop conditions
		//printf("spin = %lf\r\n", spin);
		if(spin == spin_previous && (angleRelative <= 0.1 && angleRelative >= -0.1)){
			state = POS_STATE_MOVE;
		}

		spin_previous = spin;

		break;

	case POS_STATE_MOVE:
		//printf("moveProgress = %lf\r\n", moveProgress);
		//printf("angleRelative = %lf\r\n", angleRelative);

		// Choix du sens de rotation
		MOT_SetDirection(&MoteurGauche, MOT_FUNCTIONS_FORWARD);
		MOT_SetDirection(&MoteurDroite, MOT_FUNCTIONS_FORWARD);

		// Contrôle de la vitesse
		double speed = POS_LIN_SMAX * speedCurve(moveProgress, 1);
		speed += POS_LIN_SMIN * (1 - moveProgress);
		double spCorrection = 1 - fabs(angleRelative);

		if(distanceToTarget < 50){
			CTRL_SpeedControl(&MoteurGauche, &CodeurGauche, ticksLeft, speed);
			CTRL_SpeedControl(&MoteurDroite, &CodeurDroite, ticksRight, speed);
		}
		else{
			if(angleRelative < 0){
				CTRL_SpeedControl(&MoteurGauche, &CodeurGauche, ticksLeft, speed);
				CTRL_SpeedControl(&MoteurDroite, &CodeurDroite, ticksRight, speed * spCorrection);
			} else{
				CTRL_SpeedControl(&MoteurGauche, &CodeurGauche, ticksLeft, speed * spCorrection);
				CTRL_SpeedControl(&MoteurDroite, &CodeurDroite, ticksRight, speed);
			}
		}

		// Stop conditions
		//printf("speed = %lf\r\n", speed);
		if(angleRelative < (M_PI/2) && angleRelative > (-M_PI/2) && distanceToTarget < 10){
			state = POS_STATE_STANDBY;
			targetX = 0;
			targetY = 0;
		}
		break;

	default:
		state = POS_STATE_STANDBY;
		break;
	}
}

/* End of exported functions -------------------------------------------------*/
