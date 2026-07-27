/******************************************************************************
*

* GDCC Handset V4.0
*
* Télécommande GDCC V4.0
*
* Architecture radio :
*
* * La télécommande possède son propre identifiant radio physique :
*
* ```
      RADIO_ID
  ```
*
* * Toutes les locomotives partagent la même adresse radio physique :
*
* ```
      GDCC_RADIO_NETWORK_ID
  ```
*
* * L'identification individuelle d'une locomotive est logique.
*
* ```
  Elle est transportée dans :
  ```
*
* ```
      RadioPacket.destination
  ```
*
* * Le récepteur filtre les paquets reçus en comparant :
*
* ```
      packet.destination
  ```
*
* ```
  avec :
  ```
*
* ```
      LOCO_ID
  ```
*
* Exemple :
*
* Télécommande :
* ```
    RADIO_ID = 1
  ```
*
* Réseau GDCC :
* ```
    GDCC_RADIO_NETWORK_ID = 200
  ```
*
* Loco 10 :
* ```
    LOCO_ID = 10
  ```
*
* Loco 11 :
* ```
    LOCO_ID = 11
  ```
*
* Loco 12 :
* ```
    LOCO_ID = 12
  ```
*
* Lorsque la loco 11 est sélectionnée :
*
* packet.destination = 11
*
* La trame est envoyée vers :
*
* GDCC_RADIO_NETWORK_ID
*
* Toutes les locomotives peuvent donc recevoir physiquement
* le paquet, mais seule la loco 11 traite la commande.
*

******************************************************************************/

#include <Arduino.h>

#include "Config.h"
#include "Types.h"
#include "Inputs.h"
#include "Display.h"
#include "Radio.h"

//======================================================
// Etat de la télécommande
//======================================================
//
// Cet objet contient l'état courant des commandes utilisateur.
//
// Il est utilisé par :
//
//   - Inputs_Update()
//   - Display_Update()
//   - Radio_Send()
//
//======================================================

HandsetState handset;

//======================================================
// SETUP
//======================================================

void setup()
{
//==================================================
// Port série
//==================================================


Serial.begin(9600);

delay(500);


Serial.println();

Serial.println(
    F("==============================")
);

Serial.println(
    F("GDCC HANDSET V4.0")
);

Serial.println(
    F("==============================")
);


//==================================================
// Initialisation de l'état interne
//==================================================
//
// Valeurs par défaut utilisées avant la première
// lecture des entrées.
//
// La télécommande démarre toujours avec :
//
//   - aucune vitesse
//   - direction avant
//   - éclairage éteint
//   - ARU inactif
//
//==================================================

handset.loco =
    0;

handset.potValue =
    512;

handset.throttle =
    0;

handset.directionForward =
    true;

handset.light =
    false;

handset.emergencyStop =
    false;


//==================================================
// Initialisation des entrées
//==================================================

Inputs_Init();

Serial.println(
    F("INPUTS OK")
);


//==================================================
// Lecture initiale des entrées
//==================================================
//
// Le sélecteur de locomotive est lu avant
// l'initialisation de la radio.
//
// IMPORTANT V4.0 :
//
// La locomotive sélectionnée n'est PLUS utilisée
// comme identifiant physique du NRF24L01.
//
// Elle sera simplement utilisée comme destination
// logique dans RadioPacket.destination.
//
// Exemple :
//
//   Sélection loco 10
//       ->
//   handset.loco = 10
//
//   RadioPacket.destination = 10
//
//==================================================

Inputs_Update(
    handset
);


Serial.print(
    F("LOCO SELECTIONNEE - DESTINATION LOGIQUE : ")
);

Serial.println(
    handset.loco
);


//==================================================
// Initialisation OLED
//==================================================

Display_Init();

Serial.println(
    F("OLED OK")
);


//==================================================
// Initialisation radio
//==================================================
//
// V4.0 :
//
// La radio de la télécommande est initialisée avec
// son propre identifiant physique :
//
//     RADIO_ID
//
// défini dans Config.h.
//
// Exemple :
//
//     RADIO_ID = 1
//
// L'identifiant de la locomotive sélectionnée
// n'intervient PAS dans l'initialisation du NRF24L01.
//
// La communication avec une locomotive se fera
// ensuite par :
//
//     RadioPacket.destination
//
//==================================================

Radio_Init(
    handset
);


//==================================================
// Etat initial sécurisé
//==================================================
//
// Même si le potentiomètre n'est pas au centre
// au démarrage, la télécommande commence avec
// une commande d'arrêt.
//
// Les commandes suivantes seront envoyées
// normalement après la première lecture de la boucle.
//
//==================================================

handset.throttle =
    0;

handset.directionForward =
    true;

handset.light =
    false;

handset.emergencyStop =
    false;


Serial.println(
    F("SYSTEME PRET")
);

delay(1000);


}

//======================================================
// LOOP
//======================================================

void loop()
{
//==================================================
// 1. Lecture des entrées
//==================================================
//
// Mise à jour de :
//
//   - locomotive sélectionnée
//   - potentiomètre
//   - vitesse
//   - direction
//   - éclairage
//   - ARU
//
// La locomotive sélectionnée est stockée dans :
//
//     handset.loco
//
// Elle deviendra la destination logique du prochain
// paquet radio.
//
//==================================================


Inputs_Update(
    handset
);


//==================================================
// 2. Mise à jour OLED
//==================================================
//
// La valeur brute du potentiomètre est utilisée
// uniquement pour l'affichage.
//
// Elle n'est pas transmise par radio.
//
// Le paquet radio contient uniquement :
//
//   destination
//   command
//   throttle
//   direction
//   ARU
//   LIGHT_Value
//
//==================================================

Display_Update(
    handset,
    analogRead(
        PIN_THROTTLE
    )
);


//==================================================
// 3. Transmission radio GDCC V4.0
//==================================================
//
// Le paquet est envoyé régulièrement.
//
// Cette transmission périodique est nécessaire
// au fonctionnement du timeout de sécurité
// du récepteur.
//
// Timeout récepteur :
//
//     RADIO_TIMEOUT = 500 ms
//
// Fréquence d'envoi :
//
//     environ 100 ms
//
// La fonction Radio_Send() construit la trame :
//
//     packet.destination = handset.loco
//
// puis transmet la trame vers :
//
//     GDCC_RADIO_NETWORK_ID
//
//==================================================

Radio_Send(
    handset
);


//==================================================
// 4. Moniteur série
//==================================================

Serial.print(
    F("LOCO DEST : ")
);

Serial.print(
    handset.loco
);


Serial.print(
    F(" | THROTTLE : ")
);

Serial.print(
    handset.throttle
);


Serial.print(
    F(" | DIRECTION : ")
);


if (
    handset.throttle == 0
)
{
    Serial.print(
        F("STOP")
    );
}
else if (
    handset.directionForward
)
{
    Serial.print(
        F("AVANT")
    );
}
else
{
    Serial.print(
        F("ARRIERE")
    );
}


Serial.print(
    F(" | LIGHT : ")
);


if (
    handset.light
)
{
    Serial.print(
        F("ON")
    );
}
else
{
    Serial.print(
        F("OFF")
    );
}


Serial.print(
    F(" | ARU : ")
);


if (
    handset.emergencyStop
)
{
    Serial.println(
        F("ACTIF")
    );
}
else
{
    Serial.println(
        F("INACTIF")
    );
}


//==================================================
// 5. Rafraîchissement
//==================================================
//
// Environ 100 ms entre chaque transmission.
//
// Le récepteur reçoit donc régulièrement des paquets
// valides et son timeout de sécurité reste réarmé.
//
//==================================================

delay(100);


}
