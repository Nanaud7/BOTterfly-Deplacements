#include "BOTterfly-H/config.h"

#include <unistd.h>

uint8_t Dir_Init()
{
	Dir_Ph_X = 0;
	Dir_Ph_Y = 0;
	Dir_X = 0;
	Dir_Y = 0;
	Dir_Red = 0;
	Dir_PhasePrec = 0;
	Dir_Phase = 0;
	Dir_nbPhases = 6;
	Dir_nbSL = 3;
	Dir_nbSLValidees = 0;
	return 0;
}

int Dir_PhaseChange(uint8_t Val)
{
	if ((Val > -1) && (Val < Dir_nbPhases))
	{
		Dir_PhasePrec = Dir_Phase;
		Dir_Phase = Val;
	}
	return 0;
}

uint32_t Dir_Dir()
{
	// Vérification si on va vers du rouge :
	Dir_Red = 0;
	if (RGB_Sensor.isFloorRed != 0)
	{// RED !
		Dir_Red = 1;
		Dir_PhaseChange(4);
	}

	// PHASES :

	// P0 Recherche de source lumineuse
	if (Dir_Phase == 0)
	{
		// /!\ Faire faire au robot un tour sur lui-même en enregistrant les valeurs moyennes de photodiodes tous les 1/8 tour
		// /!\ Puis lui dire d'aller vers là où les valeurs sont le plius importantes
		// /!\ Attention il faut aussi prendre en compte les sources déjà vues...
		Dir_PhaseChange(1);
	}
	// P1 Se diriger vers la source lumineuse
	else if (Dir_Phase == 1)
	{
		if ((TOF_Sensor[1].rangeMillimeter < D_SL) && (Ph_Angle > -21) && (Ph_Angle < 21))
		{// On a atteint la source lumineuse
			// /!\ Moteurs à l'arrêt
			Dir_PhaseChange(2);
		}
		else
		{// On n'a pas atteint la source lumineuse
			Dir_VectPh(); // Calcule Dir_Ph_X et Dir_Ph_Y
			Dir_X = Dir_Ph_X;
			Dir_Y = Dir_Ph_Y;
			Dir_AddToF();
			// /!\ On va là : (Dir_X ; Dir_Y)
		}
	}
	// P2 Attendre 10 s
	else if (Dir_Phase == 2)
	{
		// /!\ Moteurs à l'arrêt
		sleep(10000); // On attend les 10 secondes (on cherche pas à gagner du temps en faisant autre chose, sinon ça va mal se passer...)
		Dir_nbSLValidees++;
		// /!\ Enregistrer l'emplacement de la source pour ne plus la prendre en compte
		if (Dir_nbSL >= Dir_nbSLValidees)
		{// BOTterfly a tout trouvé
			Dir_PhaseChange(3);
		}
		else
		{// BOTterfly n'a pas tout trouvé
			Dir_PhaseChange(0);
		}
	}
	// P3 Retour au nid
	else if (Dir_Phase == 3)
	{
		// /!\ Vérifier que c'est bon pour la position de départ retour au nid
		Dir_X = 0;
		Dir_Y = 0;
		if (/*/!\ Position actuelle proche du nid (nid = Dir_X et Dir_Y du coup)*/0)
		{
			// /!\ Moteurs à l'arrêt
			Dir_PhaseChange(5);
		}
		else
		{
			Dir_AddToF();
			// /!\ On va là ... Dir_X Dir_Y
		}
	}
	// P4 Zone rouge
	else if (Dir_Phase == 4)
	{
		// /!\ Moteurs à l'arrêt (Solution temporaire, solution envisagée : nouvelle phase avec parcours "préconçu")
	}
	// P5 Terminé
	else if (Dir_Phase == 5)
	{
		// /!\ Moteurs à l'arrêt
		// /!\ Faire clignoter la lampe du capteur de luminère
	}
	return /*TABLEAU X Y*/0;
}

uint8_t Dir_VectPh()
{
	int Norme = 0;
	// On prend la plus fiable :
	if (TOF_Sensor[1].rangeMillimeter <= 3500)
	{
		Norme = TOF_Sensor[1].rangeMillimeter;
	}
	else
	{
		Norme = Ph_Norma;
	}
	Dir_Ph_X = (int)((Norme + R_BOT + MARGE) * cos(Ph_Angle));
	Dir_Ph_Y = (int)((Norme+ R_BOT + MARGE) * sin(Ph_Angle));
	return 0;
}

uint8_t Dir_AddToF()
{
	// Modification de la trajectoire si les TOF détectent un objet proche :
	if (TOF_Sensor[2].rangeMillimeter < D_TOF)
	{ // cos(-40) = 0,766 | sin(-40) = -0,643
		Dir_X += -(int)((R_BOT + MARGE)*0.766);
		Dir_Y += -(int)((R_BOT + MARGE)*-0.643);
	}
	if (TOF_Sensor[1].rangeMillimeter < D_TOF)
	{ // cos(0) = 1 | sin(0) = 0
		Dir_X += -(int)((R_BOT + MARGE));
		Dir_Y += 0;
	}
	if (TOF_Sensor[0].rangeMillimeter < D_TOF)
	{ // cos(40) = ... | sin(40) = ...
		Dir_X += -(int)((R_BOT + MARGE)*0.766);
		Dir_Y += -(int)((R_BOT + MARGE)*0.643);
	}
	return 0;
}







