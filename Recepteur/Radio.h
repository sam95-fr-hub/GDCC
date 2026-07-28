/******************************************************************************
*
* GDCC RECEPTEUR
* Communication radio NRF24L01
*
* Version V4.1
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
// Vérification de la réception d'un paquet
//======================================================

bool Radio_Available();


//======================================================
// Réception d'un paquet radio
//
// V4.1
//
// Un paquet est accepté si :
//
//     packet.destination == LOCO_ID
//
// ou :
//
//     packet.destination == GDCC_DEST_BROADCAST
//
// Le broadcast est utilisé pour les commandes globales
// de sécurité, notamment l'ARU.
//
// Les commandes broadcast normales ne sont pas
// acceptées comme commandes de conduite.
//======================================================

bool Radio_Receive(
    RadioPacket &packet
);

#endif