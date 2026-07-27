/******************************************************************************
*

* GDCC RECEPTEUR
* Structures de données du récepteur
*
* Version V4.0
*

******************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

//======================================================
// Commandes GDCC
//======================================================

// Commande normale de conduite
#define CMD_DRIVE              1

// Réservé V4.1
#define CMD_EMERGENCY_STOP     2

// Réservé V4.2
#define CMD_MEM_ON             3
#define CMD_MEM_OFF            4

//======================================================
// Destination broadcast
//
// Réservée aux futures commandes globales.
//======================================================

#define GDCC_DEST_BROADCAST    255

//======================================================
// Paquet de commande radio GDCC V4.0
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
// Exemples :
//   10 = Loco 10
//   11 = Loco 11
//   12 = Loco 12
//
// 255 :
//   destination broadcast réservée aux futures
//   commandes globales.
//
// IMPORTANT :
//
// Ce champ correspond à l'identifiant logique
// de la locomotive et non à l'adresse physique
// du NRF24L01.
//==================================================

```
uint8_t destination;


//==================================================
// Type de commande
//
// V4.0 :
//   CMD_DRIVE
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
//   0 = marche arrière
//   1 = marche avant
//==================================================

uint8_t direction;


//==================================================
// Arrêt d'urgence
//
//   0 = fonctionnement normal
//   1 = ARU actif
//
// Le champ est transmis dans la trame V4.0.
//==================================================

uint8_t ARU;


//==================================================
// Eclairage
//
//   0 = éteint
//   1 = allumé
//==================================================

uint8_t LIGHT_Value;
```

};

#endif
