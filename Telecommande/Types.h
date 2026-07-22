```cpp
/******************************************************************************
 *
 * GDCC
 * Structures de données de la télécommande
 *
 * Version V3.1
 * Architecture modulaire
 *
 ******************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>


//======================================================
// Etat interne de la télécommande
//
// Ces données sont utilisées par la télécommande
// pour gérer les commandes et l'affichage.
//
// IMPORTANT :
// potValue est une valeur interne à la télécommande.
// Elle n'est PAS transmise par radio.
//
// La valeur transmise est "throttle" (0 à 255).
//======================================================

struct HandsetState
{
    //==================================================
    // Locomotive sélectionnée
    //
    // Correspond à l'identifiant radio de la locomotive
    //==================================================

    uint8_t loco;


    //==================================================
    // Valeur brute du potentiomètre
    //
    // Valeur ADC Arduino :
    //   0    = butée arrière
    //   512  = position centrale
    //   1023 = butée avant
    //
    // Cette valeur reste interne à la télécommande.
    // Elle peut être utilisée pour l'affichage ou
    // le diagnostic.
    //==================================================

    int potValue;


    //==================================================
    // Vitesse normalisée
    //
    // Valeur utilisée pour la transmission radio :
    //
    //   0   = arrêt
    //   255 = vitesse maximale
    //
    // La conversion de la valeur du potentiomètre
    // 0-1023 vers 0-255 est réalisée dans la
    // télécommande avant la transmission.
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
    //==================================================

    bool emergencyStop;
};


//======================================================
// Paquet de commande radio
//
// Protocole GDCC V3.1
//
// Ce paquet doit être STRICTEMENT IDENTIQUE
// à la structure RadioPacket du récepteur.
//
// Taille : 4 octets
//
// Octet 0 : throttle
// Octet 1 : direction
// Octet 2 : ARU
// Octet 3 : LIGHT_Value
//
// La valeur brute du potentiomètre (0-1023) n'est
// volontairement PAS transmise.
//
// La télécommande effectue la conversion avant
// l'envoi du paquet.
//======================================================

struct RadioPacket
{
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
```
