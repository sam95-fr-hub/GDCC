```cpp
/******************************************************************************
 *
 * GDCC Handset V3.1
 *
 * Télécommande pour récepteurs GDCC V3.1
 *
 * Protocole radio :
 *
 *   throttle    : 0 à 255
 *   direction   : 0 = arrière / 1 = avant
 *   ARU         : 0 = normal / 1 = actif
 *   LIGHT_Value : 0 = éteint / 1 = allumé
 *
 * La valeur brute du potentiomètre 0-1023 reste
 * uniquement interne à la télécommande.
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
        F("GDCC HANDSET V3.1")
    );

    Serial.println(
        F("==============================")
    );


    //==================================================
    // Initialisation de l'état
    //
    // Valeurs par défaut avant lecture des entrées.
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
    //
    // Cette lecture est effectuée AVANT l'initialisation
    // de la radio afin de connaître immédiatement
    // l'identifiant de la locomotive sélectionnée.
    //
    // Le sélecteur 12 positions détermine :
    //
    //   position 1  -> ID 10
    //   position 2  -> ID 11
    //   ...
    //   position 12 -> ID 21
    //
    // La radio sera donc initialisée directement avec
    // le bon identifiant.
    //==================================================

    Inputs_Update(
        handset
    );


    Serial.print(
        F("LOCO SELECTIONNEE - ID RADIO : ")
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
    //
    // La radio est maintenant initialisée avec
    // l'identifiant réel de la locomotive sélectionnée.
    //==================================================

    Radio_Init(
        handset
    );


    //==================================================
    // Etat initial sécurisé
    //
    // La locomotive démarre toujours à l'arrêt.
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

    Inputs_Update(
        handset
    );


    //==================================================
    // 2. Mise à jour OLED
    //==================================================

    Display_Update(
        handset,
        analogRead(
            PIN_THROTTLE
        )
    );


    //==================================================
    // 3. Envoi radio
    //
    // Le paquet est envoyé toutes les 100 ms.
    //
    // Cette transmission régulière est nécessaire
    // au fonctionnement du timeout de sécurité du
    // récepteur V3.1.
    //
    // Timeout récepteur :
    //   500 ms
    //
    // Fréquence d'envoi :
    //   100 ms
    //==================================================

    Radio_Send(
        handset
    );


    //==================================================
    // 4. Moniteur série
    //==================================================

    Serial.print(
        F("LOCO ID : ")
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
    // Rafraîchissement
    //
    // 100 ms entre chaque transmission.
    //==================================================

    delay(100);
}
```
