/******************************************************************************
 *
 * GDCC
 * Gestion de la communication radio NRF24L01
 *
 ******************************************************************************/

#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include "Types.h"

//======================================================
// Initialisation du module radio
//======================================================

bool Radio_Init();

//======================================================
// Envoi d'un paquet
//======================================================

bool Radio_Send(
    uint8_t destinationId,
    const RadioPacket &packet
);

#endif