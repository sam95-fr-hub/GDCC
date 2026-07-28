/******************************************************************************
*
* GDCC
* Structures de données de la télécommande
*
* Version V4.1
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
    //==================================================

    uint8_t loco;


    //==================================================
    // Valeur brute du potentiomètre
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
    // Arrêt d'urgence global
    //
    //   false = fonctionnement normal
    //   true  = ARU global actif
    //
    // Lorsque true, Radio_Send() doit transmettre
    // exclusivement une commande CMD_EMERGENCY_STOP
    // en broadcast vers toutes les locomotives.
    //==================================================

    bool emergencyStop;
};


//======================================================
// Commandes GDCC V4.1
//======================================================

//======================================================
// Commande normale de conduite
//
// Destination : locomotive sélectionnée
//======================================================

#define CMD_DRIVE                  1


//======================================================
// Entrée en arrêt d'urgence global
//
// Destination : GDCC_DEST_BROADCAST
//======================================================

#define CMD_EMERGENCY_STOP         2


//======================================================
// Réservé V4.2
//
// Activation du système de mémorisation.
//======================================================

#define CMD_MEM_ON                 3


//======================================================
// Réservé V4.2
//
// Désactivation du système de mémorisation.
//======================================================

#define CMD_MEM_OFF                4


//======================================================
// Sortie de l'arrêt d'urgence global
//
// Destination : GDCC_DEST_BROADCAST
//
// Cette commande libère l'état ARU mais ne doit pas
// provoquer la reprise automatique d'une locomotive.
//======================================================

#define CMD_EMERGENCY_RELEASE      5


//======================================================
// Destinations GDCC
//======================================================

//======================================================
// Destination normale
//
// Correspond à l'identifiant logique de la locomotive.
//
// 255 = broadcast global vers toutes les locomotives.
//======================================================

#define GDCC_DEST_BROADCAST        255


//======================================================
// Paquet de commande radio GDCC V4.1
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
    // 255 = broadcast global.
    //==================================================

    uint8_t destination;


    //==================================================
    // Type de commande
    //==================================================

    uint8_t command;


    //==================================================
    // Vitesse
    //==================================================

    uint8_t throttle;


    //==================================================
    // Direction
    //==================================================

    uint8_t direction;


    //==================================================
    // Arrêt d'urgence global
    //
    //   0 = fonctionnement normal
    //   1 = ARU global actif
    //
    // En V4.1, lorsqu'un paquet contient
    // CMD_EMERGENCY_STOP, la destination doit être
    // GDCC_DEST_BROADCAST.
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