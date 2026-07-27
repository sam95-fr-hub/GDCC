/******************************************************************************
*

* GDCC
* Configuration matérielle de la télécommande
*
* Version V4.0
*

******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//======================================================
// OLED
//======================================================

#define OLED_WIDTH             128
#define OLED_HEIGHT            64

//======================================================
// Entrées analogiques
//======================================================

const uint8_t PIN_SELECTOR = A0;
const uint8_t PIN_THROTTLE = A1;
const uint8_t PIN_BATTERY  = A2;

//======================================================
// Boutons
//======================================================

const uint8_t PIN_ESTOP = 2;
const uint8_t PIN_LIGHT = 3;

// Les deux prochains seront ajoutés plus tard

// const uint8_t PIN_F1 = ?;
// const uint8_t PIN_F2 = ?;

//======================================================
// NRF24L01
//======================================================

//======================================================
// Identifiant physique de la télécommande
//
// Cet identifiant est utilisé uniquement pour configurer
// l'adresse radio physique du NRF24L01 de la télécommande.
//
// Il ne correspond PAS à l'identifiant d'une locomotive.
//======================================================

const uint8_t RADIO_ID = 1;

//======================================================
// Adresse physique commune du réseau GDCC
//
// Toutes les locomotives utilisent cette même adresse
// radio physique.
//
// La télécommande transmet les commandes vers cette
// adresse.
//
// L'identification individuelle de la locomotive est
// ensuite transportée dans :
//
//     RadioPacket.destination
//
// et filtrée par chaque récepteur selon son :
//
//     LOCO_ID
//======================================================

const uint8_t GDCC_RADIO_NETWORK_ID = 200;

//======================================================
// Broches du module NRF24L01
//======================================================

const uint8_t PIN_NRF_CE  = 9;
const uint8_t PIN_NRF_CSN = 10;

//======================================================
// Canal radio
//
// Le canal doit être identique sur la télécommande
// et sur tous les récepteurs GDCC.
//
// Canal 100 = fréquence radio 2500 MHz.
//======================================================

const uint8_t RADIO_CHANNEL = 100;

//======================================================
// Batterie
//======================================================

// Pont diviseur

const float BATTERY_R1 = 4700.0;
const float BATTERY_R2 = 6800.0;

//======================================================
// Potentiomètre
//======================================================

// Zone morte autour du centre

const int DEAD_ZONE = 40;

#endif
