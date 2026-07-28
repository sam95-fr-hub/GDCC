/******************************************************************************
 *
 * GDCC
 * Gestion du moteur locomotive
 *
 * Version V4.1
 * Architecture modulaire
 *
 ******************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>


//======================================================
// Initialisation du moteur
//======================================================

void Motor_Init();


//======================================================
// Réglage de la direction
//
// direction :
//   0 = arrière
//   1 = avant
//======================================================

void Motor_SetDirection(
    uint8_t direction
);


//======================================================
// Réglage de la vitesse
//
// throttle :
//   0   = arrêt
//   255 = vitesse maximale
//======================================================

void Motor_SetSpeed(
    uint8_t throttle
);


//======================================================
// Arrêt immédiat du moteur
//
// Utilisé notamment par :
//
//   - ARU broadcast
//   - batterie faible
//   - perte de liaison radio
//
// L'arrêt ne modifie pas la direction mémorisée.
// La vitesse interne est remise à zéro.
//======================================================

void Motor_Stop();


#endif