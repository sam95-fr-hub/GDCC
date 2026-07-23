
/******************************************************************************
 *
 * GDCC
 * Configuration du récepteur locomotive
 *
 * Version V3.2
 * Architecture modulaire
 *
 * La configuration de ce fichier est spécifique à chaque locomotive.
 *
 ******************************************************************************/

#ifndef CONFIG_LOCO12_H
#define CONFIG_LOCO12_H

//======================================================
// IDENTIFICATION DE LA LOCOMOTIVE
//======================================================
//
// A modifier pour chaque locomotive.
//
// Exemple :
// Loco 1  -> 10
// Loco 2  -> 11
// Loco 3  -> 12
//
// Cette valeur doit correspondre à l'ID utilisé
// par la télécommande GDCC.
//

#define RADIO_ID 12


//======================================================
// CONFIGURATION RADIO NRF24L01
//======================================================
//
// Pour un NRF24L01 externe :
// modifier CE et CSN selon le câblage.
//
// Pour une RF-Nano :
// utiliser les broches du NRF24L01 intégré.
//
// Exemple RF-Nano :
// CE  = D7
// CSN = D8
//
// Exemple Arduino Nano + NRF24L01 externe :
// CE  = 9 ou 10 selon câblage
// CSN = 9 ou 10 selon câblage
//

#define PIN_RADIO_CE  7
#define PIN_RADIO_CSN 8


//======================================================
// SECURITE - TIMEOUT RADIO
//======================================================
//
// Si aucun paquet radio valide n'est reçu pendant
// cette durée, le récepteur passe en sécurité.
//
// 500 ms = 0,5 seconde
//

#define RADIO_TIMEOUT 500UL


//======================================================
// CONFIGURATION DU DRIVER MOTEUR
//======================================================
//
// Décommenter UN SEUL driver.
//
// Pour un L298N :
// #define MOTOR_DRIVER_L298N
//
// Pour un DRV8871 :
// #define MOTOR_DRIVER_DRV8871
//

//#define MOTOR_DRIVER_L298N
#define MOTOR_DRIVER_DRV8871


//======================================================
// SENS PHYSIQUE DU MOTEUR
//======================================================
//
// false = sens normal
// true  = sens inversé
//
// Ce réglage permet d'inverser le sens du moteur
// sans modifier Motor.cpp.
//
// Si les fils du moteur sont inversés physiquement,
// il suffit de modifier cette valeur.
//

#define MOTOR_REVERSED false


//======================================================
// BROCHES DU DRIVER MOTEUR
//======================================================
//
// L298N :
//   IN1 = D4
//   IN2 = D3
//   ENA = D5
//
// DRV8871 :
//   IN1 = D5
//   IN2 = D3
//
// La broche ENA n'est pas utilisée avec le DRV8871.
//
// Les broches inutilisées peuvent rester définies.
//

#define PIN_MOTOR_IN1 5
#define PIN_MOTOR_IN2 3
#define PIN_MOTOR_ENA 5


//======================================================
// CONFIGURATION DE L'ECLAIRAGE
//======================================================
//
// Choisir UN SEUL mode d'éclairage.
//
//
// MODE 0 : PAS D'ECLAIRAGE
// --------------------------------------------
// Aucun éclairage n'est installé sur la locomotive.
//
// La réception de LIGHT_Value est ignorée.
//
// #define LIGHT_MODE_NONE
//
//
// MODE 1 : ECLAIRAGE VIA L298N
// --------------------------------------------
// Utilisé lorsqu'un L298N possède un deuxième pont
// disponible pour commander un éclairage.
//
// IN3 et IN4 du L298N sont utilisés comme sorties.
//
// #define LIGHT_MODE_L298N
//
//
// MODE 2 : LED DIRECTEMENT SUR UNE SORTIE ARDUINO
// --------------------------------------------
// Utilisé pour commander une LED depuis une sortie
// numérique de l'Arduino.
//
// ATTENTION :
// La LED doit obligatoirement être équipée d'une
// résistance de limitation de courant adaptée.
//
// La sortie Arduino ne doit pas alimenter directement
// une ampoule ou une charge importante.
//
// #define LIGHT_MODE_LED
//
//======================================================
//
// POUR LA LOCO 10 ACTUELLE :
//
// Aucun éclairage physique n'est installé.
//
// On sélectionne donc :
//

#define LIGHT_MODE_NONE


//======================================================
// ECLAIRAGE VIA L298N
//======================================================
//
// Ces broches sont utilisées uniquement si :
//
// #define LIGHT_MODE_L298N
//
// IN3 et IN4 correspondent au deuxième pont du L298N.
//
// Exemple :
// IN3 = D6
// IN4 = D7
//
// Si ces broches sont utilisées par autre chose,
// modifier les valeurs ci-dessous.
//

#define PIN_LIGHT_IN3 6
#define PIN_LIGHT_IN4 7


//======================================================
// ECLAIRAGE LED DIRECT SUR ARDUINO
//======================================================
//
// Cette broche est utilisée uniquement si :
//
// #define LIGHT_MODE_LED
//
// La LED doit être connectée avec une résistance
// de limitation de courant.
//
// Exemple :
// LED -> résistance -> D6
// Cathode LED -> GND
//
// Modifier cette valeur si nécessaire.
//

#define PIN_LIGHT_LED 6


//======================================================
// MESURE DE BATTERIE
//======================================================
//
// true  = mesure batterie disponible
// false = aucune mesure batterie
//
// Pour une locomotive sans pont diviseur :
// #define BATTERY_MONITOR_ENABLED false
//
// Exemple RF-Nano sans mesure batterie.
//

#define BATTERY_MONITOR_ENABLED false


//======================================================
// ENTREE MESURE BATTERIE
//======================================================
//
// Utilisée uniquement si :
//
// BATTERY_MONITOR_ENABLED == true
//
// Batterie -> R1 -> A2 -> R2 -> GND
//

#define PIN_BATTERY A2


//======================================================
// PONT DIVISEUR DE TENSION
//======================================================
//
// R1 = 10 kOhm
// R2 = 6.8 kOhm
//
// Batterie -> R1 -> A2 -> R2 -> GND
//

#define BATTERY_R1 10000.0
#define BATTERY_R2 6800.0


//======================================================
// SEUIL BATTERIE MINIMUM
//======================================================
//
// Coupure de sécurité à 6.4 V
//
// Utilisé uniquement si la mesure batterie
// est activée.
//

#define BATTERY_MIN_VOLTAGE 6.4


//======================================================
// INTERVALLE DE VERIFICATION BATTERIE
//======================================================
//
// 1000 ms = 1 seconde
//

#define BATTERY_CHECK_INTERVAL 1000UL


#endif
