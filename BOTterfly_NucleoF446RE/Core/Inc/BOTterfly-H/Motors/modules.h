
#ifndef INC_CONFIG_H_
#define INC_CONFIG_H

#include <BOTterfly-H/Motors/motor.h>
#include <BOTterfly-H/Motors/encoder.h>
#include <BOTterfly-H/Motors/odometry.h>
#include <BOTterfly-H/Motors/shellOS.h>

/* MOTORS */
struct MOT_HandleTypeDef MoteurGauche;
struct MOT_HandleTypeDef MoteurDroite;

/* ENCODERS */
struct ENC_HandleTypeDef CodeurGauche;
struct ENC_HandleTypeDef CodeurDroite;

#endif /* INC_CONFIG_H_ */
