/******************************************************************************
 *
 * GDCC
 * Gestion de l'affichage OLED
 *
 ******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "Types.h"

//======================================================
// Initialisation
//======================================================

void Display_Init();


//======================================================
// Affichage principal
//======================================================

void Display_Update(
    const HandsetState &state,
    const char* locoName,
    int potValue
);

#endif