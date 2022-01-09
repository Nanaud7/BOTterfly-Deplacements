/**
 ******************************************************************************
 * @file	strategy.c
 * @author 	Arnaud CHOBERT
 * @brief	Stratégie de BOTterfly
 ******************************************************************************
 */

#include "BOTterfly-H/strategy.h"

/* Types ---------------------------------------------------------------------*/
/* End of types --------------------------------------------------------------*/

/* Macros --------------------------------------------------------------------*/

/* End of macros -------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
Strategy strategy[NB_OF_POINTS];
uint8_t numberOfPoints = NB_OF_POINTS;
uint8_t indexStrategy = 1;

/* End of variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void strategyInit(){
	strategy[0].x = 0;
	strategy[0].y = 1;
	strategy[0].dir = 1;

	strategy[1].x = 100;
	strategy[1].y = 100;
	strategy[1].dir = 1;

	strategy[2].x = 200;
	strategy[2].y = 0;
	strategy[2].dir = 1;

	strategy[3].x = 0;
	strategy[3].y = 200;
	strategy[3].dir = 1;

	strategy[4].x = 0;
	strategy[4].y = 1;
	strategy[4].dir = 1;
}

/* End of exported functions -------------------------------------------------*/
