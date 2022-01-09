/**
 ******************************************************************************
 * @file	asserv_vit.c
 * @author 	Arnaud CHOBERT
 * @brief	Speed loop control for BOTterfly
 ******************************************************************************
 */

#include <BOTterfly-H/asserv_vit.h>

/* Macros --------------------------------------------------------------------*/
#define SPEED_CONTROL_FREQUENCY 50
#define SPEED_CONTROL_PERIOD (1/(float)SPEED_CONTROL_FREQUENCY)

/* End of macros -------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
/* End of variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * VIT_SpeedControl : Speed control function called every 20ms
 * @param Motor Motor HandleTypeDef
 * @param Encoder Encoder HandleTypeDef
 * @return TBD
 */
int32_t VIT_SpeedControl(MOT_HandleTypeDef* Motor, ENC_HandleTypeDef* Encoder,
		int32_t ticks, double consigne){

	// Force tick to positive value
	if(ticks < 0) ticks = ticks * (-1);

	// Speed calculation
	int freq_codeuse = SPEED_CONTROL_FREQUENCY * ticks;
	//float speed = ((float)freq_codeuse * ((float)WHEEL_PERIMETER/Encoder->TicksPerRev));
	float speed = ((float)freq_codeuse * Encoder->TicksCoeff); // mm/s

	float erreur = consigne - speed;
	float s = 60; // Command offset
	s = s + Motor->Kp * erreur;
	s = s + Motor->Ki * (erreur + Motor->err_previous);
	//s = s + Motor->s_previous + Motor->Ki * (erreur + Motor->err_previous) * (SPEED_CONTROL_PERIOD/2);

	if(s >= 80) 	s = 80;
	else if(s < 0) 	s = 0;

	Motor->s_previous = s;
	Motor->err_previous = erreur;

	MOT_SetDutyCycle(Motor,s);

	return speed;
}

/* End of functions ----------------------------------------------------------*/
