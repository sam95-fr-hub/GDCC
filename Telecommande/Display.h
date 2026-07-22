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
// Affichage principal
//======================================================

void Display_Update(
    const HandsetState &state,
    int potValue
);


#endif