/******************************************************************************
*

* GDCC RECEPTEUR
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
//======================================================
//
// Le NRF24L01 du récepteur est configuré pour écouter
// l'adresse radio physique commune du réseau GDCC.
//
// Cette adresse est définie dans Config.h par :
//
//     GDCC_RADIO_NETWORK_ID
//
// Toutes les locomotives du réseau utilisent cette
// même adresse radio physique.
//
// L'identité logique de chaque locomotive est définie
// par :
//
//     LOCO_ID
//
// dans le fichier Config.h correspondant.
//
// Le canal radio est défini dans Config.h par :
//
//     RADIO_CHANNEL
//
// L'ID logique de la locomotive et l'adresse physique
// du réseau sont donc deux notions distinctes.
//
//     Adresse physique NRF24L01
//         = GDCC_RADIO_NETWORK_ID
//
//     Identité logique locomotive
//         = LOCO_ID
//
//     Destination dans le paquet
//         = RadioPacket.destination
//======================================================

void Radio_Init();

//======================================================
// Vérification de la réception d'un paquet
//======================================================
//
// Retourne :
//
//     true  = un paquet radio est disponible
//     false = aucun paquet disponible
//
// IMPORTANT :
//
// En V4.0, toutes les locomotives du réseau écoutent
// la même adresse radio physique.
//
// Le filtrage par LOCO_ID est effectué par
// Radio_Receive().
//
// Radio_Available() ne vérifie donc pas si le paquet
// est destiné à cette locomotive.
//======================================================

bool Radio_Available();

//======================================================
// Réception d'un paquet radio
//======================================================
//
// Retourne :
//
//     true  = paquet reçu et destiné à cette locomotive
//     false = aucun paquet ou paquet destiné à une
//             autre locomotive
//
// Un paquet destiné à une autre locomotive est lu
// puis ignoré.
//
// V4.0 :
//
// Seule la commande CMD_DRIVE est acceptée.
//
// Le paquet est reçu sans ACK côté radio.
//
// Le récepteur ne renvoie donc aucune confirmation
// automatique à la télécommande.
//======================================================

bool Radio_Receive(
RadioPacket &packet
);

#endif
