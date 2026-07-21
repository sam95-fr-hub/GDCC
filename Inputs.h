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
// Mise à jour de l'état de la télécommande
//======================================================

void Inputs_Update(HandsetState &state);


#endif