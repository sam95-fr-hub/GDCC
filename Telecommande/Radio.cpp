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

#include "Radio.h"
#include "Config.h"

#include <SPI.h>
#include <NRFLite.h>


//======================================================
// Objet radio NRF24L01
//======================================================

static NRFLite radio;


//======================================================
// Identifiant radio actuellement configuré
//
// 0 = aucun ID encore configuré
//
// L'ID réel des locomotives doit être différent de 0.
//======================================================

static uint8_t currentRadioId = 0;


//======================================================
// Indique si le module radio a été initialisé
//======================================================

static bool radioInitialized = false;


//======================================================
// Initialisation du module radio
//======================================================

void Radio_Init(
    const HandsetState &state
)
{
    //==================================================
    // Initialisation du NRF24L01
    //
    // L'identifiant radio correspond à la locomotive
    // actuellement sélectionnée.
    //==================================================

    if (
        !radio.init(
            state.loco,
            PIN_NRF_CE,
            PIN_NRF_CSN
        )
    )
    {
        Serial.println(
            F("ERREUR : NRF24L01 NON DETECTE")
        );


        //================================================
        // Blocage en cas d'erreur radio
        //================================================

        while (1)
        {
            // Sécurité :
            // la télécommande ne démarre pas
            // sans communication radio.
        }
    }


    //==================================================
    // Mémorisation de l'ID actif
    //==================================================

    currentRadioId =
        state.loco;


    radioInitialized =
        true;


    //==================================================
    // Diagnostic
    //==================================================

    Serial.print(
        F("NRF24L01 OK - RADIO ID : ")
    );

    Serial.println(
        currentRadioId
    );
}


//======================================================
// Changement de locomotive
//
// Cette fonction reconfigure le NRF24L01 avec le nouvel
// identifiant radio.
//
// Elle permet de changer de locomotive à la volée,
// sans redémarrer la télécommande.
//======================================================

static void Radio_SelectLoco(
    uint8_t newRadioId
)
{
    //==================================================
    // Vérification
    //==================================================

    if (
        newRadioId == currentRadioId
    )
    {
        return;
    }


    //==================================================
    // Diagnostic
    //==================================================

    Serial.print(
        F("CHANGEMENT LOCO : ")
    );

    Serial.print(
        currentRadioId
    );

    Serial.print(
        F(" -> ")
    );

    Serial.println(
        newRadioId
    );


    //==================================================
    // Réinitialisation du NRF24L01
    //
    // Le nouvel ID devient l'adresse radio utilisée
    // pour les transmissions suivantes.
    //==================================================

    if (
        !radio.init(
            newRadioId,
            PIN_NRF_CE,
            PIN_NRF_CSN
        )
    )
    {
        Serial.println(
            F("ERREUR : IMPOSSIBLE DE SELECTIONNER LA LOCO")
        );

        return;
    }


    //==================================================
    // Mémorisation du nouvel ID
    //==================================================

    currentRadioId =
        newRadioId;


    Serial.print(
        F("RADIO CONFIGUREE - LOCO ID : ")
    );

    Serial.println(
        currentRadioId
    );
}


//======================================================
// Transmission d'un paquet radio
//======================================================

void Radio_Send(
    const HandsetState &state
)
{
    //==================================================
    // Vérification de l'initialisation
    //==================================================

    if (
        !radioInitialized
    )
    {
        return;
    }


    //==================================================
    // Vérification du changement de locomotive
    //
    // Si le sélecteur 12 positions a changé de position,
    // l'identifiant radio change également.
    //
    // Le NRF24L01 est alors automatiquement reconfiguré.
    //==================================================

    if (
        state.loco != currentRadioId
    )
    {
        Radio_SelectLoco(
            state.loco
        );
    }


    //==================================================
    // Création du paquet GDCC V3.1
    //==================================================

    RadioPacket packet;


    //==================================================
    // Vitesse
    //
    // La télécommande transmet directement la valeur
    // normalisée 0-255.
    //
    // La valeur brute du potentiomètre 0-1023
    // n'est plus transmise.
    //==================================================

    packet.throttle =
        state.throttle;


    //==================================================
    // Direction
    //
    // false = arrière = 0
    // true  = avant  = 1
    //==================================================

    packet.direction =
        state.directionForward
        ? 1
        : 0;


    //==================================================
    // Arrêt d'urgence
    //
    // false = normal = 0
    // true  = ARU    = 1
    //==================================================

    packet.ARU =
        state.emergencyStop
        ? 1
        : 0;


    //==================================================
    // Eclairage
    //
    // false = éteint = 0
    // true  = allumé = 1
    //==================================================

    packet.LIGHT_Value =
        state.light
        ? 1
        : 0;


    //==================================================
    // Transmission radio
    //==================================================

    radio.send(
        state.loco,
        &packet,
        sizeof(packet)
    );
}
```
