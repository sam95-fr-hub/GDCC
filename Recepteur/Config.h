/******************************************************************************
 *
 * GDCC
 * Configuration du récepteur locomotive
 *
 * Version V3.1
 * Architecture modulaire
 *
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


//======================================================
// IDENTIFIANT RADIO
//
// A modifier pour chaque locomotive
// Exemple :
// Loco 1  -> 10
// Loco 2  -> 11
// Loco 3  -> 12
//======================================================

#define RADIO_ID 11


//======================================================
// NRF24L01
//======================================================

#define PIN_RADIO_CE  10
#define PIN_RADIO_CSN 9


//======================================================
// L298N - MOTEUR
//======================================================

#define PIN_MOTOR_IN1 4
#define PIN_MOTOR_IN2 3
#define PIN_MOTOR_ENA 5


//======================================================
// L298N - ECLAIRAGE
//======================================================

#define PIN_LIGHT_IN3 6
#define PIN_LIGHT_IN4 7


//======================================================
// MESURE BATTERIE
//======================================================

#define PIN_BATTERY A2


//======================================================
// PONT DIVISEUR DE TENSION
//
// R1 = 10 kOhm
// R2 = 6.8 kOhm
//
// Batterie -> R1 -> A2 -> R2 -> GND
//======================================================

#define BATTERY_R1 10000.0
#define BATTERY_R2 6800.0


//======================================================
// SEUIL BATTERIE MINIMUM
//
// Coupure de sécurité à 6.4 V
//======================================================

#define BATTERY_MIN_VOLTAGE 6.4


//======================================================
// INTERVALLE DE VERIFICATION BATTERIE
//
// 1000 ms = 1 seconde
//======================================================

#define BATTERY_CHECK_INTERVAL 1000UL


#endif