/*
 * photodiodes.h
 *
 *  Created on: Dec 3, 2021
 *      Author: quent
 */

#ifndef INC_BOTTERFLY_H_DIRECTION_H_
#define INC_BOTTERFLY_H_DIRECTION_H_

// Tout en mm :
#define D_TOF 400 // Distance max pour considérer les détections comme obstacles
#define R_BOT 110 // Rayon de BOTterfly
#define MARGE 100 // Marge d'évitement d'obstacles
#define D_SL 200 // Distance min source lumineuse

/* Variables Globales ------------------------------------------------------------------*/

int Dir_Ph_X;
int Dir_Ph_Y;
int Dir_X;
int Dir_Y;
uint8_t Dir_Red;
uint8_t Dir_PhasePrec;
uint8_t Dir_Phase;
uint8_t Dir_nbPhases;
uint8_t Dir_nbSL;
uint8_t Dir_nbSLValidees;
/* Phases :
0 : Recherche de source lumineuse
1 : Se diriger vers la source lumineuse
v2 : Attendre 10 s
v3 : Retour au nid
v4 : Zone rouge
v5 : Terminé
*/
uint8_t Dir_Init();

int Dir_PhaseChange(uint8_t Val);

uint32_t Dir_Dir();

uint8_t Dir_VectPh();

uint8_t Dir_AddToF();

#endif /* INC_BOTTERFLY_H_DIRECTION_H_ */
