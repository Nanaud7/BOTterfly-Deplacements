/*
 * config.h
 *
 *  Created on: Dec 3, 2021
 *      Author: jerem
 */

#ifndef INC_BOTTERFLY_H_CONFIG_H_
#define INC_BOTTERFLY_H_CONFIG_H_

#include "tof_sensor.h"
#include "rgb_sensor.h"
#include "photodiodes.h"
#include "direction.h"

#include "Motors/modules.h"
#include "Motors/motor.h"
#include "Motors/encoder.h"
#include "Motors/ctrl_speed.h"
#include "Motors/ctrl_positionV1.h"
#include "Motors/odometry.h"

/* TOF SENSORS */

struct VL53L0X_Dev_t TOF_Sensor[TOF_nbOfSensor];

/* RGB SENSORS */
struct RGB_struct RGB_Sensor;

#endif /* INC_BOTTERFLY_H_CONFIG_H_ */

