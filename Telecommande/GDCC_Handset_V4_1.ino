 /******************************************************************************
*
* GDCC Handset V4.1
*
* Télécommande GDCC V4.1
*
* Architecture radio :
*
* La télécommande possède son propre identifiant radio physique :
*
*     RADIO_ID
*
* Toutes les locomotives partagent la même adresse radio physique :
*
*     GDCC_RADIO_NETWORK_ID
*
* L'identification individuelle d'une locomotive est logique.
*
* Elle est transportée dans :
*
*     RadioPacket.destination
*
* Une commande normale CMD_DRIVE est envoyée vers la locomotive
* sélectionnée.
*
* Une commande CMD_EMERGENCY_STOP est envoyée en broadcast :
*
*     destination = GDCC_DEST_BROADCAST
*
* Toutes les locomotives reçoivent alors l'ordre d'arrêt d'urgence.
*
* Tant que l'ARU est actif, la télécommande transmet uniquement
* CMD_EMERGENCY_STOP en broadcast.
*
* Une commande CMD_EMERGENCY_RELEASE est envoyée en broadcast
* lors de la sortie de l'ARU.
*
* La désactivation de l'ARU ne provoque aucun redémarrage
* automatique des locomotives.
*
* Après désactivation, chaque locomotive doit recevoir une
* nouvelle commande CMD_DRIVE normale pour reprendre son
* fonctionnement.
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
        F("GDCC HANDSET V4.1")
    );

    Serial.println(
        F("==============================")
    );


    //==================================================
    // Initialisation de l'état interne
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

    Radio_Init(
        handset
    );


    //==================================================
    // Etat initial sécurisé
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
    // 3. Transmission radio GDCC V4.1
    //==================================================
    //
    // Fonctionnement normal :
    //
    //     destination = handset.loco
    //     command     = CMD_DRIVE
    //
    // ARU actif :
    //
    //     destination = GDCC_DEST_BROADCAST
    //     command     = CMD_EMERGENCY_STOP
    //
    // L'ARU est transmis périodiquement en broadcast
    // tant que handset.emergencyStop est actif.
    //
    // Sortie ARU :
    //
    //     destination = GDCC_DEST_BROADCAST
    //     command     = CMD_EMERGENCY_RELEASE
    //
    // Cette commande est envoyée une seule fois lors
    // de la transition ARU actif -> ARU inactif.
    //
    // La sortie de l'ARU ne provoque aucun ordre de
    // marche automatique.
    //
    // La locomotive doit attendre un nouvel ordre
    // CMD_DRIVE.
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


    //==================================================
    // ARU actif
    //
    // La destination réellement utilisée par la radio
    // est le broadcast global.
    //==================================================

    if (
        handset.emergencyStop
    )
    {
        Serial.print(
            GDCC_DEST_BROADCAST
        );
    }
    else
    {
        Serial.print(
            handset.loco
        );
    }


    Serial.print(
        F(" | CMD : ")
    );


    //==================================================
    // Commande réellement déterminée par l'état
    // de la télécommande.
    //
    // Pendant l'ARU :
    //
    //     CMD_EMERGENCY_STOP
    //
    // En fonctionnement normal :
    //
    //     CMD_DRIVE
    //
    // La transition de sortie d'ARU envoie
    // ponctuellement CMD_EMERGENCY_RELEASE dans
    // Radio_Send(), puis le fonctionnement normal
    // reprend à la transmission suivante.
    //==================================================

    if (
        handset.emergencyStop
    )
    {
        Serial.print(
            CMD_EMERGENCY_STOP
        );
    }
    else
    {
        Serial.print(
            CMD_DRIVE
        );
    }


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
            F("ACTIF - BROADCAST")
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
    // En ARU :
    //
    //     CMD_EMERGENCY_STOP
    //     destination = GDCC_DEST_BROADCAST
    //
    // est envoyé périodiquement.
    //
    // Lors de la sortie d'ARU :
    //
    //     CMD_EMERGENCY_RELEASE
    //     destination = GDCC_DEST_BROADCAST
    //
    // est envoyé une seule fois par Radio_Send().
    //
    //==================================================

    delay(100);
}