/******************************************************************************
 *
 * GDCC
 * Communication radio NRF24L01
 *
 * Version V3.1
 * Architecture modulaire
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
//
// Retourne :
//   true  = paquet disponible
//   false = aucun paquet
//======================================================

bool Radio_Available();


//======================================================
// Réception d'un paquet radio
//
// Retourne :
//   true  = réception réussie
//   false = erreur
//======================================================

bool Radio_Receive(
    RadioPacket &packet
);


#endif