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

    uint16_t throttle;

    bool directionForward;

    bool light;

    bool emergencyStop;
};


//======================================================
// Paquet radio compatible V2.x
//
// IMPORTANT :
// Cette structure reste strictement compatible
// avec les récepteurs des locomotives V2.x
//======================================================

struct RadioPacket
{
    int POT_Value;

    int ARU;

    int LIGHT_Value;
};


#endif