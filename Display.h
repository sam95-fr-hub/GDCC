/******************************************************************************
 *
 * GDCC
 * Gestion de l'affichage OLED
 *
 ******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Types.h"


//======================================================
// Initialisation OLED
//======================================================

void Display_Init();


//======================================================
// Mise à jour de l'affichage principal
//
// Affiche uniquement :
// - Radio ID locomotive
// - Throttle
// - Direction
// - LIGHT
// - ARU
//
// Les noms de locomotives sont supprimés.
//======================================================

void Display_Update(
    const HandsetState &state,
    int potValue
);


#endif