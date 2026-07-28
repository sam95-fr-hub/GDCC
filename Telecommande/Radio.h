/******************************************************************************
*
* GDCC
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
//
// Initialise le NRF24L01 de la télécommande.
//
// RADIO_ID : identifiant radio physique de la
//            télécommande.
//
// GDCC_RADIO_NETWORK_ID : adresse radio physique
//                         commune aux locomotives.
//
// state.loco : identifiant logique de la locomotive
//              actuellement sélectionnée.
//======================================================

void Radio_Init(
    const HandsetState &state
);


//======================================================
// Transmission d'un paquet radio
//======================================================
//
// Fonctionnement normal :
//
//     destination = state.loco
//     command     = CMD_DRIVE
//
// ARU actif :
//
//     destination = GDCC_DEST_BROADCAST
//     command     = CMD_EMERGENCY_STOP
//
//     L'ARU est envoyé périodiquement en broadcast
//     tant que l'état emergencyStop est actif.
//
// Sortie ARU :
//
//     destination = GDCC_DEST_BROADCAST
//     command     = CMD_EMERGENCY_RELEASE
//
//     Cette commande est envoyée lors de la transition
//     ARU actif -> ARU inactif.
//
// La sortie d'ARU ne constitue pas un ordre de marche.
// Les locomotives doivent rester arrêtées et attendre
// un nouvel ordre CMD_DRIVE.
//
// Priorité des commandes :
//
//     1. CMD_EMERGENCY_STOP en broadcast
//     2. CMD_EMERGENCY_RELEASE en broadcast
//     3. CMD_DRIVE vers la locomotive sélectionnée
//
// Lorsque l'ARU est actif, aucune commande CMD_DRIVE
// ne doit être envoyée.
//
// La transmission utilise NRFLite::NO_ACK.
//======================================================

void Radio_Send(
    const HandsetState &state
);

#endif