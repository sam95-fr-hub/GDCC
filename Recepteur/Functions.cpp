/******************************************************************************
 *
 * GDCC
 * Gestion des fonctions locomotive
 *
 * Version V3.1
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
    // Configuration des broches L298N
    // utilisées pour l'éclairage
    //==================================================

    pinMode(
        PIN_LIGHT_IN3,
        OUTPUT
    );

    pinMode(
        PIN_LIGHT_IN4,
        OUTPUT
    );


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
    // Activation de la sortie éclairage
    //==================================================

    digitalWrite(
        PIN_LIGHT_IN3,
        HIGH
    );

    digitalWrite(
        PIN_LIGHT_IN4,
        LOW
    );
}


//======================================================
// Extinction de l'éclairage
//======================================================

void Functions_LightOff()
{
    //==================================================
    // Désactivation de la sortie éclairage
    //==================================================

    digitalWrite(
        PIN_LIGHT_IN3,
        LOW
    );

    digitalWrite(
        PIN_LIGHT_IN4,
        LOW
    );
}