/******************************************************************************
 *
 * GDCC
 * Communication radio NRF24L01
 *
 * Compatible avec les récepteurs V2.x
 *
 ******************************************************************************/

#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include "Types.h"


//======================================================
// Initialisation du module radio
//======================================================

void Radio_Init();


//======================================================
// Envoi du paquet de commande
//======================================================
//
// Le format RadioPacket reste compatible V2.x
//
// - POT_Value
// - ARU
// - LIGHT_Value
//
//======================================================

void Radio_Send(
    const HandsetState &state
);


#endif