/******************************************************************************
*

* GDCC RECEPTEUR
* Structures de données du récepteur
*
* Version V4.1
*

******************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

//======================================================
// Commandes GDCC
//======================================================

// Commande normale de conduite
#define CMD_DRIVE                  1

// Entrée en arrêt d'urgence global
#define CMD_EMERGENCY_STOP         2

// Réservé V4.2
#define CMD_MEM_ON                 3

// Réservé V4.2
#define CMD_MEM_OFF                4

// Sortie de l'arrêt d'urgence global
#define CMD_EMERGENCY_RELEASE      5

//======================================================
// Destination broadcast
//======================================================
//
// 255 = commande destinée à toutes les locomotives
//
// Utilisé notamment pour les commandes d'arrêt
// d'urgence global.
//======================================================

#define GDCC_DEST_BROADCAST        255

//======================================================
// Paquet de commande radio GDCC V4.1
//
// DOIT être strictement identique à celui de la
// télécommande.
//
// Taille : 6 octets
//
// Octet 0 : destination
// Octet 1 : command
// Octet 2 : throttle
// Octet 3 : direction
// Octet 4 : ARU
// Octet 5 : LIGHT_Value
//======================================================

struct RadioPacket
{
//==================================================
// Destination logique
//
// ID de la locomotive destinataire.
//
// 255 = destination broadcast.
//==================================================


uint8_t destination;


//==================================================
// Type de commande
//
// CMD_DRIVE
// CMD_EMERGENCY_STOP
// CMD_EMERGENCY_RELEASE
//==================================================

uint8_t command;


//==================================================
// Vitesse
//
//   0   = arrêt
//   255 = vitesse maximale
//==================================================

uint8_t throttle;


//==================================================
// Direction
//
//   0 = arrière
//   1 = avant
//==================================================

uint8_t direction;


//==================================================
// Arrêt d'urgence
//
//   0 = fonctionnement normal
//   1 = ARU actif
//
// Pour les commandes broadcast ARU, la commande
// elle-même indique l'état de l'ARU.
//==================================================

uint8_t ARU;


//==================================================
// Eclairage
//
//   0 = éteint
//   1 = allumé
//==================================================

uint8_t LIGHT_Value;


};

#endif
