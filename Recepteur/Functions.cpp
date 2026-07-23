
/******************************************************************************
 *
 * GDCC
 * Gestion des fonctions locomotive
 *
 * Version V3.2
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
    //==================================================
    // Configuration selon le type d'éclairage
    //==================================================

    #if defined(LIGHT_MODE_L298N)

        //==============================================
        // Eclairage via deuxième pont du L298N
        //==============================================

        pinMode(
            PIN_LIGHT_IN3,
            OUTPUT
        );

        pinMode(
            PIN_LIGHT_IN4,
            OUTPUT
        );

    #elif defined(LIGHT_MODE_LED)

        //==============================================
        // Eclairage LED directement sur Arduino
        //==============================================

        pinMode(
            PIN_LIGHT_LED,
            OUTPUT
        );

    #endif


    //==================================================
    // Etat initial sécurisé
    //==================================================

    Functions_LightOff();
}


//======================================================
// Allumage de l'éclairage
//======================================================

void Functions_LightOn()
{
    //==================================================
    // Eclairage via L298N
    //==================================================

    #if defined(LIGHT_MODE_L298N)

        digitalWrite(
            PIN_LIGHT_IN3,
            HIGH
        );

        digitalWrite(
            PIN_LIGHT_IN4,
            LOW
        );


    //==================================================
    // Eclairage LED directement sur Arduino
    //==================================================

    #elif defined(LIGHT_MODE_LED)

        digitalWrite(
            PIN_LIGHT_LED,
            HIGH
        );


    //==================================================
    // Aucun éclairage
    //==================================================

    #elif defined(LIGHT_MODE_NONE)

        // Rien à faire.


    #endif
}


//======================================================
// Extinction de l'éclairage
//======================================================

void Functions_LightOff()
{
    //==================================================
    // Eclairage via L298N
    //==================================================

    #if defined(LIGHT_MODE_L298N)

        digitalWrite(
            PIN_LIGHT_IN3,
            LOW
        );

        digitalWrite(
            PIN_LIGHT_IN4,
            LOW
        );


    //==================================================
    // Eclairage LED directement sur Arduino
    //==================================================

    #elif defined(LIGHT_MODE_LED)

        digitalWrite(
            PIN_LIGHT_LED,
            LOW
        );


    //==================================================
    // Aucun éclairage
    //==================================================

    #elif defined(LIGHT_MODE_NONE)

        // Rien à faire.


    #endif
}
