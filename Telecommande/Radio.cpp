
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
    // RADIO_ID est l'identifiant propre de la
    // télécommande.
    //
    // L'adresse de destination sera ensuite définie
    // par state.loco lors de l'envoi.
    //==================================================

    if (
        !radio.init(
            RADIO_ID,
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
    // Mémorisation de l'ID de la locomotive active
    //
    // La radio est initialisée avec son propre ID,
    // puis les transmissions sont envoyées vers
    // state.loco.
    //==================================================

    currentRadioId =
        state.loco;


    radioInitialized =
        true;


    //==================================================
    // Diagnostic
    //==================================================

    Serial.print(
        F("NRF24L01 OK - EMETTEUR ID : ")
    );

    Serial.println(
        RADIO_ID
    );


    Serial.print(
        F("LOCO DESTINATION ID : ")
    );

    Serial.println(
        currentRadioId
    );
}


//======================================================
// Changement de locomotive
//
// IMPORTANT :
// Le NRF24L01 de l'émetteur conserve son propre
// identifiant RADIO_ID.
//
// Il n'est PAS réinitialisé avec l'ID de la locomotive.
//
// L'ID de la locomotive est uniquement utilisé comme
// adresse de destination lors de radio.send().
//
// Cette méthode est identique au fonctionnement V2
// qui fonctionne avec les locomotives 10, 11 et 12.
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
    // Mémorisation du nouvel ID de destination
    //
    // IMPORTANT :
    // On ne fait PAS de radio.init() ici.
    //
    // La radio reste configurée avec RADIO_ID = 1.
    //==================================================

    currentRadioId =
        newRadioId;


    Serial.print(
        F("DESTINATION RADIO CONFIGUREE - LOCO ID : ")
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
    // l'adresse de destination change également.
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
    // 0   = arrêt
    // 255 = vitesse maximale
    //==================================================

    packet.throttle =
        state.throttle;


    //==================================================
    // Direction
    //
    // 0 = arrière
    // 1 = avant
    //==================================================

    packet.direction =
        state.directionForward
        ? 1
        : 0;


    //==================================================
    // Arrêt d'urgence
    //
    // 0 = normal
    // 1 = ARU actif
    //==================================================

    packet.ARU =
        state.emergencyStop
        ? 1
        : 0;


    //==================================================
    // Eclairage
    //
    // 0 = éteint
    // 1 = allumé
    //==================================================

    packet.LIGHT_Value =
        state.light
        ? 1
        : 0;


    //==================================================
    // Transmission radio
    //
    // L'émetteur utilise toujours RADIO_ID = 1.
    //
    // La locomotive sélectionnée est l'adresse
    // de destination.
    //==================================================

    radio.send(
        currentRadioId,
        &packet,
        sizeof(packet)
    );
}
