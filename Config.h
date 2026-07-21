/******************************************************************************
 *
 * GDCC
 * Configuration matérielle
 *
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

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

//const uint8_t PIN_F1 = ?;
//const uint8_t PIN_F2 = ?;

//======================================================
// NRF24L01
//======================================================

// Identifiant radio de la télécommande
const uint8_t RADIO_ID = 1;

// Broches du module NRF24L01
const uint8_t PIN_NRF_CE  = 9;
const uint8_t PIN_NRF_CSN = 10;

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

//======================================================
// Radio
//======================================================

const uint8_t RADIO_CHANNEL = 100;

#endif