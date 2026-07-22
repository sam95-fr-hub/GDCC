/******************************************************************************
 *
 * GDCC
 * Gestion des fonctions locomotive
 *
 * Version V3.1
 * Architecture modulaire
 *
 ******************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>


//======================================================
// Initialisation des fonctions
//
// Configuration des sorties du L298N utilisées
// pour commander l'éclairage de la locomotive.
//======================================================

void Functions_Init();


//======================================================
// Allumage de l'éclairage
//======================================================

void Functions_LightOn();


//======================================================
// Extinction de l'éclairage
//======================================================

void Functions_LightOff();


#endif