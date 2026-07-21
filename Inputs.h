/******************************************************************************
 *
 * GDCC
 * Gestion des entrées de la télécommande
 *
 ******************************************************************************/

#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "Types.h"


//======================================================
// Initialisation des entrées
//======================================================

void Inputs_Init();


//======================================================
// Mise à jour des entrées de commande
//
// Lit :
// - Sélecteur locomotive
// - Throttle
// - E-STOP
// - LIGHT
//
// La batterie n'est PAS lue ici.
//======================================================

void Inputs_Update(HandsetState &state);


//======================================================
// Lecture de la tension batterie
//
// Cette fonction est appelée uniquement au démarrage
// de la télécommande.
//
// Retour : tension batterie réelle en volts.
//======================================================

float Inputs_ReadBatteryVoltage();


#endif