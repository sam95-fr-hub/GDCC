/******************************************************************************
 *
 * GDCC
 * Gestion des fonctions locomotive
 *
 * Version V4.1
 * Architecture modulaire
 *
 ******************************************************************************/

#include "Functions.h"
#include "Config.h"


//======================================================
// Initialisation des fonctions
//======================================================

void Functions_Init()
{
    #if defined(LIGHT_MODE_L298N)

        pinMode(
            PIN_LIGHT_IN3,
            OUTPUT
        );

        pinMode(
            PIN_LIGHT_IN4,
            OUTPUT
        );

    #elif defined(LIGHT_MODE_LED)

        pinMode(
            PIN_LIGHT_LED,
            OUTPUT
        );

    #endif


    Functions_LightOff();
}


//======================================================
// Allumage de l'éclairage
//======================================================

void Functions_LightOn()
{
    #if defined(LIGHT_MODE_L298N)

        digitalWrite(
            PIN_LIGHT_IN3,
            HIGH
        );

        digitalWrite(
            PIN_LIGHT_IN4,
            LOW
        );


    #elif defined(LIGHT_MODE_LED)

        digitalWrite(
            PIN_LIGHT_LED,
            HIGH
        );


    #elif defined(LIGHT_MODE_NONE)

        // Aucun éclairage.


    #endif
}


//======================================================
// Extinction de l'éclairage
//======================================================

void Functions_LightOff()
{
    #if defined(LIGHT_MODE_L298N)

        digitalWrite(
            PIN_LIGHT_IN3,
            LOW
        );

        digitalWrite(
            PIN_LIGHT_IN4,
            LOW
        );


    #elif defined(LIGHT_MODE_LED)

        digitalWrite(
            PIN_LIGHT_LED,
            LOW
        );


    #elif defined(LIGHT_MODE_NONE)

        // Aucun éclairage.


    #endif
}