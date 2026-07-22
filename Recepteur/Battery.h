/******************************************************************************
 *
 * GDCC
 * Gestion de la batterie locomotive
 *
 * Version V3.1
 * Architecture modulaire
 *
 ******************************************************************************/

#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>


//======================================================
// Initialisation de la mesure de batterie
//======================================================

void Battery_Init();


//======================================================
// Lecture de la tension batterie
//
// Retourne la tension réelle de la batterie en volts.
//
// Exemple :
//   8.2 V
//   7.4 V
//   6.4 V
//======================================================

float Battery_ReadVoltage();


//======================================================
// Vérification du niveau de batterie
//
// Retourne :
//   true  = batterie faible
//   false = tension normale
//======================================================

bool Battery_IsLow();


#endif