/**
 ******************************************************************************
 * @file	strategy.h
 * @author 	Arnaud CHOBERT
 * @brief	Stratégie de BOTterfly
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BOTTERFLY_H_STRATEGY_H_
#define INC_BOTTERFLY_H_STRATEGY_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
	double x;
	double y;
	uint8_t dir;
} Strategy;

/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
#define NB_OF_POINTS 5
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern Strategy strategy[NB_OF_POINTS];
extern uint8_t numberOfPoints;
extern uint8_t indexStrategy;

/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void STRAT_strategyInit();
/* End of exported functions -------------------------------------------------*/

#endif /* INC_BOTTERFLY_H_STRATEGY_H_ */
