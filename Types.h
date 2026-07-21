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
// Etat complet de la télécommande
//======================================================

struct HandsetState
{
    uint8_t loco;

    int16_t throttle;

    bool directionForward;

    bool light;

    bool emergencyStop;

    float batteryVoltage;
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