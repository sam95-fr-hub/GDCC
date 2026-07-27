/******************************************************************************
*

* GDCC
* Structures de données de la télécommande
*
* Version V4.0
*

******************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

//======================================================
// Etat interne de la télécommande
//======================================================

struct HandsetState
{
//==================================================
// Locomotive sélectionnée
//
// Correspond à l'identifiant logique réel
// de la locomotive.
//
// Exemples :
//   10 = Loco 10
//   11 = Loco 11
//   12 = Loco 12
//
// Cet identifiant est utilisé comme destination
// logique dans RadioPacket.destination.
//==================================================

```
uint8_t loco;


//==================================================
// Valeur brute du potentiomètre
//
// Valeur ADC Arduino :
//
//   0    = butée arrière
//   512  = position centrale
//   1023 = butée avant
//
// Cette valeur reste interne à la télécommande.
//==================================================

int potValue;


//==================================================
// Vitesse normalisée
//
//   0   = arrêt
//   255 = vitesse maximale
//==================================================

uint8_t throttle;


//==================================================
// Direction
//
//   false = marche arrière
//   true  = marche avant
//==================================================

bool directionForward;


//==================================================
// Eclairage
//
//   false = éteint
//   true  = allumé
//==================================================

bool light;


//==================================================
// Arrêt d'urgence
//
//   false = fonctionnement normal
//   true  = ARU actif
//
// V4.0 :
// Le champ est conservé dans la trame.
//
// Le traitement global de l'ARU sera traité
// dans une version ultérieure.
//==================================================

bool emergencyStop;
```

};

//======================================================
// Commandes GDCC
//
// V4.0 utilise uniquement CMD_DRIVE.
//
// Les autres commandes sont réservées aux futures
// versions du protocole.
//======================================================

#define CMD_DRIVE              1

// Réservé V4.1
#define CMD_EMERGENCY_STOP     2

// Réservé V4.2
#define CMD_MEM_ON             3
#define CMD_MEM_OFF            4

//======================================================
// Destinations GDCC
//======================================================

// Destination normale :
// ID logique de la locomotive.
//
// Exemples :
//   10
//   11
//   12
//
// Une destination 255 est réservée aux futures
// commandes globales diffusées à toutes les
// locomotives.

#define GDCC_DEST_BROADCAST    255

//======================================================
// Paquet de commande radio GDCC V4.0
//
// Ce paquet doit être STRICTEMENT IDENTIQUE
// dans la télécommande et dans le récepteur.
//
// Taille : 6 octets
//
// Octet 0 : destination
// Octet 1 : command
// Octet 2 : throttle
// Octet 3 : direction
// Octet 4 : ARU
// Octet 5 : LIGHT_Value
//
// V4.0 :
//   destination = ID logique de la locomotive
//
// V4.1 :
//   destination = ID logique de la locomotive
//              ou GDCC_DEST_BROADCAST (255)
//
// V4.2 :
//   de nouvelles commandes pourront être ajoutées.
//======================================================

struct RadioPacket
{
//==================================================
// Destination logique
//
// ID de la locomotive destinataire.
//
// Exemple :
//   10 = Loco 10
//   11 = Loco 11
//   12 = Loco 12
//
// 255 = BROADCAST
//       réservé aux futures commandes globales.
//
// IMPORTANT :
//
// Ce champ est indépendant de l'adresse radio
// physique GDCC_RADIO_NETWORK_ID.
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
