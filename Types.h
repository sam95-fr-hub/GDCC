/******************************************************************************
 *
 * GDCC
 * Structures communes
 *
 ******************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>


//======================================================
// Etat de la télécommande
//======================================================

struct HandsetState
{
    uint8_t loco;

    // Valeur brute du potentiomètre
    // Compatible avec le fonctionnement V2.x
    int potValue;

    // Vitesse calculée
    uint16_t throttle;

    bool directionForward;

    bool light;

    bool emergencyStop;
};


//======================================================
// Paquet radio compatible V2.x
//======================================================

struct RadioPacket
{
    int POT_Value;

    int ARU;

    int LIGHT_Value;
};


#endif