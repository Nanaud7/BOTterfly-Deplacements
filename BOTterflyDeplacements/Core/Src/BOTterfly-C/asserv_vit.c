/**
 ******************************************************************************
 * @file	asserv_vit.c
 * @author 	Arnaud CHOBERT
 * @brief	Asservissement en vitesse de BOTterfly
 ******************************************************************************
 */

#include <BOTterfly-H/asserv_vit.h>

#define SPEED_CONTROL_FREQUENCY 50
#define SPEED_CONTROL_PERIOD (1/(float)SPEED_CONTROL_FREQUENCY)

/* Variables -----------------------------------------------------------------*/
ASSERV_VIT_HandleTypeDef SpeedControl;
double consigneVit = 100;
//double acc = 0; // 0

/* End of variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * Define which timer is used for speed control loop
 * @param htim TIM HandleTypeDef
 * @return 0 if HAL_OK
 */
uint8_t vit_SetTimer(TIM_HandleTypeDef *htim){
	SpeedControl.Timer = htim;

	if(HAL_OK != HAL_TIM_Base_Start_IT(SpeedControl.Timer)){
		return 1;
	}

	return 0;
}

/**
 * Set the speed wanted
 * @param new_Consigne Speed wanted
 * @return 0
 */
uint8_t vit_SetConsigne(double new_Consigne){
	consigneVit = new_Consigne;
	printf("Consigne updated = %f\r\n",consigneVit);

	return 0;
}

/**
 * Speed control loop function called with a 50Hz frequency
 * @param Motor Motor HandleTypeDef
 * @param Encoder Encoder HandleTypeDef
 * @return TBD
 */
int32_t vit_SpeedControl(MOT_HandleTypeDef* Motor, ENC_HandleTypeDef* Encoder,
		int32_t ticks, double consigne){
	// Récupération du nombre de ticks
	int32_t ticksMem = ticks;
	if(ticks < 0) ticks = ticks * (-1);

	// Calcul de la vitesse du moteur
	int freq_codeuse = SPEED_CONTROL_FREQUENCY * ticks;
	//float speed = ((float)freq_codeuse * ((float)WHEEL_PERIMETER/Encoder->TicksPerRev));
	float speed = ((float)freq_codeuse * Encoder->TicksCoeff);
	// Vitesse calculée en mm/sec

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
