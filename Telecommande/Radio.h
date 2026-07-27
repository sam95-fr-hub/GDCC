/******************************************************************************
*

* GDCC
* Communication radio NRF24L01
*
* Version V4.0
*

******************************************************************************/

#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>

#include "Types.h"

//======================================================
// Initialisation du module radio
//
// V4.0
//
// Le NRF24L01 de la télécommande est initialisé avec
// son propre identifiant radio.
//
// Les locomotives utilisent toutes la même adresse
// radio physique de réseau.
//
// L'identifiant logique de la locomotive n'est PLUS
// utilisé comme adresse NRF24L01.
//
// Il est transmis dans :
//
//     RadioPacket.destination
//
// L'adresse radio physique commune est définie dans
// Config.h par :
//
//     GDCC_RADIO_NETWORK_ID
//
// Le canal radio commun est défini dans Config.h par :
//
//     RADIO_CHANNEL
//======================================================

void Radio_Init(
const HandsetState &state
);

//======================================================
// Transmission d'un paquet radio
//
// Protocole GDCC V4.0
//
// La trame contient :
//
//   destination
//   command
//   throttle
//   direction
//   ARU
//   LIGHT_Value
//
// En V4.0, command = CMD_DRIVE.
//
// La destination correspond à l'ID logique de la
// locomotive sélectionnée.
//
// Exemple :
//
//   destination = 10
//
// La trame est envoyée vers l'adresse radio physique
// commune utilisée par tous les récepteurs.
//
// La transmission utilise NRFLite::NO_ACK.
//
// Le paquet est donc envoyé une seule fois sans
// demander d'accusé de réception au récepteur.
//
// Les retransmissions automatiques du NRF24L01 ne sont
// pas utilisées pour les commandes GDCC V4.0.
//======================================================

void Radio_Send(
const HandsetState &state
);

#endif
