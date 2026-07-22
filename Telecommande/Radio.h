```cpp
/******************************************************************************
 *
 * GDCC
 * Communication radio NRF24L01
 *
 * Version V3.1
 * Architecture modulaire
 *
 ******************************************************************************/

#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include "Types.h"


//======================================================
// Initialisation du module radio
//
// Initialise le NRF24L01 avec l'identifiant de la
// locomotive sélectionnée.
//
// L'identifiant de la locomotive est fourni par
// HandsetState.loco.
//======================================================

void Radio_Init(
    const HandsetState &state
);


//======================================================
// Transmission d'un paquet radio
//
// Protocole GDCC V3.1
//
// Le paquet transmis contient :
//
//   throttle    : 0 à 255
//   direction   : 0 ou 1
//   ARU         : 0 ou 1
//   LIGHT_Value : 0 ou 1
//
// La valeur brute du potentiomètre 0-1023 n'est
// plus transmise.
//======================================================

void Radio_Send(
    const HandsetState &state
);


#endif
```
