/******************************************************************************
 *
 * GDCC
 * Configuration générale du récepteur locomotive
 *
 * Version V4.0
 * Architecture multi-locomotives
 *
 * Ce fichier sélectionne automatiquement la configuration
 * de la locomotive à compiler.
 *
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


//======================================================
// SELECTION DE LA LOCOMOTIVE
//======================================================
//
// Modifier UNIQUEMENT cette valeur pour compiler
// le programme destiné à une autre locomotive.
//
// 10 = Config_Loco10.h
// 11 = Config_Loco11.h
// 12 = Config_Loco12.h
//
// Exemple :
//
// #define LOCO_CONFIG 10
//
// compile la configuration de la loco 10.
//
//======================================================

#define LOCO_CONFIG 12


//======================================================
// CHARGEMENT DE LA CONFIGURATION
//======================================================
//
// Le bon fichier Config_LocoXX.h est chargé
// automatiquement selon LOCO_CONFIG.
//
//======================================================

#if LOCO_CONFIG == 10

    #include "Config_Loco10.h"

#elif LOCO_CONFIG == 11

    #include "Config_Loco11.h"

#elif LOCO_CONFIG == 12

    #include "Config_Loco12.h"

#else

    #error "ERREUR : LOCO_CONFIG invalide. Utiliser 10, 11 ou 12."

#endif


//======================================================
// RESEAU RADIO GDCC
//======================================================
//
// Adresse physique commune du réseau radio GDCC.
//
// Cette valeur doit être STRICTEMENT IDENTIQUE à celle
// utilisée par la télécommande.
//
// Télécommande :
//
//     GDCC_RADIO_NETWORK_ID = 200
//
// Tous les récepteurs utilisent également :
//
//     GDCC_RADIO_NETWORK_ID = 200
//
// L'identifiant individuel de chaque locomotive n'est
// PAS cette adresse.
//
// Il est défini dans Config_LocoXX.h avec :
//
//     #define RADIO_ID XX
//
// Exemple pour la loco 12 :
//
//     RADIO_ID = 12
//
// La télécommande envoie alors un paquet contenant :
//
//     packet.destination = 12
//
// vers l'adresse radio physique commune :
//
//     GDCC_RADIO_NETWORK_ID = 200
//
// Chaque récepteur reçoit le paquet et vérifie ensuite
// si la destination correspond à son propre RADIO_ID.
//
//======================================================

#define GDCC_RADIO_NETWORK_ID 200


//======================================================
// CANAL RADIO
//======================================================
//
// Le canal doit être STRICTEMENT IDENTIQUE sur :
//
//   - la télécommande
//   - toutes les locomotives
//
// NRFLite accepte les canaux de 0 à 125.
//
// Configuration actuelle :
//
//     RADIO_CHANNEL = 100
//
//======================================================

#define RADIO_CHANNEL 100


#endif