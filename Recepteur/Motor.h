/******************************************************************************
 *
 * GDCC
 * Gestion du moteur locomotive
 *
 * Version V3.1
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
//======================================================

void Motor_Stop();


#endif