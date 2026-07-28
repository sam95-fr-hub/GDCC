/******************************************************************************
*

* GDCC
* Communication radio NRF24L01
*
* Version V4.1
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
// Etat d'initialisation de la radio
//======================================================

static bool radioInitialized = false;

//======================================================
// Etat précédent de l'ARU
//
// Permet de détecter les transitions d'état :
//
// false -> true  : entrée en ARU
// true  -> false : sortie d'ARU
//
//======================================================

static bool previousEmergencyStop = false;

//======================================================
// Etat de la séquence de sortie d'ARU
//
// true  = répétition de CMD_EMERGENCY_RELEASE active
// false = aucune répétition en cours
//
//======================================================

static bool emergencyReleaseActive = false;

//======================================================
// Etat de verrouillage après sortie d'ARU
//
// true  = attente d'une nouvelle action manuelle
// false = transmission normale autorisée
//
// Ce verrou empêche la télécommande d'envoyer
// automatiquement un CMD_DRIVE avec l'ancien état
// du potentiomètre après la sortie de l'ARU.
//
//======================================================

static bool waitingForManualCommand = false;

//======================================================
// Etat manuel mémorisé avant la sortie d'ARU
//
// Ces valeurs permettent de détecter une nouvelle
// action de conduite après la sortie de l'ARU.
//
//======================================================

static uint8_t releaseLoco = 0;

static uint8_t releaseThrottle = 0;

static bool releaseDirectionForward = true;

//======================================================
// Moment de début de la séquence RELEASE
//======================================================

static unsigned long emergencyReleaseStartTime = 0;

//======================================================
// Durée de répétition du CMD_EMERGENCY_RELEASE
//
// La fonction Radio_Send() est appelée environ toutes
// les 100 ms.
//
// 1000 ms correspondent donc à environ 10 transmissions.
//
//======================================================

#define EMERGENCY_RELEASE_DURATION 1000UL

//======================================================
// Initialisation du module radio
//======================================================

void Radio_Init(
const HandsetState &state
)
{
if (
!radio.init(
RADIO_ID,
PIN_NRF_CE,
PIN_NRF_CSN,
NRFLite::BITRATE2MBPS,
RADIO_CHANNEL
)
)
{
Serial.println(
F("ERREUR : NRF24L01 NON DETECTE")
);


    while (1)
    {
    }
}


radioInitialized = true;


Serial.println(
    F("NRF24L01 OK - EMETTEUR INITIALISE")
);


Serial.print(
    F("EMETTEUR RADIO ID : ")
);

Serial.println(
    RADIO_ID
);


Serial.print(
    F("LOCO DESTINATION : ")
);

Serial.println(
    state.loco
);


Serial.print(
    F("RESEAU RADIO GDCC : ")
);

Serial.println(
    GDCC_RADIO_NETWORK_ID
);


Serial.print(
    F("CANAL RADIO : ")
);

Serial.println(
    RADIO_CHANNEL
);


//==================================================
// Initialisation des états internes
//==================================================

previousEmergencyStop =
    state.emergencyStop;


emergencyReleaseActive =
    false;


waitingForManualCommand =
    false;


emergencyReleaseStartTime =
    0;


releaseLoco =
    state.loco;


releaseThrottle =
    state.throttle;


releaseDirectionForward =
    state.directionForward;


}

//======================================================
// Transmission d'un paquet radio
//======================================================

void Radio_Send(
const HandsetState &state
)
{
if (
!radioInitialized
)
{
return;
}


RadioPacket packet;


//==================================================
// DETECTION DE L'ENTREE EN ARU
//==================================================
//
// Lorsque l'ARU devient actif, toute éventuelle
// séquence de sortie est annulée.
//
// Le verrou de reprise manuelle est également
// annulé car l'ARU redevient prioritaire.
//
//==================================================

if (
    !previousEmergencyStop
    &&
    state.emergencyStop
)
{
    emergencyReleaseActive =
        false;


    waitingForManualCommand =
        false;
}


//==================================================
// DETECTION DE LA SORTIE D'ARU
//==================================================
//
// Transition :
//
//     ARU actif -> ARU inactif
//
// Une séquence de répétition du RELEASE est lancée.
//
// L'état manuel présent au moment de la sortie
// est mémorisé afin d'empêcher une reprise
// automatique avec l'ancien ordre.
//
//==================================================

if (
    previousEmergencyStop
    &&
    !state.emergencyStop
)
{
    emergencyReleaseActive =
        true;


    waitingForManualCommand =
        true;


    emergencyReleaseStartTime =
        millis();


    releaseLoco =
        state.loco;


    releaseThrottle =
        state.throttle;


    releaseDirectionForward =
        state.directionForward;


    Serial.println(
        F("ARU OFF - REPETITION RELEASE PENDANT 1 SEC")
    );
}


//==================================================
// PRIORITE 1
//
// ARU ACTIF
//
// Tant que l'ARU est actif, aucune commande
// CMD_DRIVE ne doit être envoyée.
//
// CMD_EMERGENCY_STOP est envoyé en broadcast.
//
//==================================================

if (
    state.emergencyStop
)
{
    packet.destination =
        GDCC_DEST_BROADCAST;

    packet.command =
        CMD_EMERGENCY_STOP;

    packet.throttle =
        0;

    packet.direction =
        0;

    packet.ARU =
        1;

    packet.LIGHT_Value =
        0;
}


//==================================================
// PRIORITE 2
//
// REPETITION DE LA SORTIE D'ARU
//
// CMD_EMERGENCY_RELEASE est envoyé en broadcast
// pendant environ 1 seconde.
//
// Aucun CMD_DRIVE n'est envoyé pendant cette
// période.
//
//==================================================

else if (
    emergencyReleaseActive
)
{
    if (
        millis() -
        emergencyReleaseStartTime >=
        EMERGENCY_RELEASE_DURATION
    )
    {
        //==========================================
        // Fin de la répétition RELEASE
        //==========================================

        emergencyReleaseActive =
            false;


        Serial.println(
            F("FIN REPETITION RELEASE - ATTENTE NOUVELLE ACTION")
        );


        //==========================================
        // Aucun CMD_DRIVE ne doit être envoyé ici.
        //
        // Le verrou waitingForManualCommand reste
        // actif jusqu'à une nouvelle action manuelle.
        //==========================================

        packet.destination =
            GDCC_DEST_BROADCAST;

        packet.command =
            CMD_EMERGENCY_RELEASE;

        packet.throttle =
            0;

        packet.direction =
            0;

        packet.ARU =
            0;

        packet.LIGHT_Value =
            0;
    }
    else
    {
        //==========================================
        // Répétition RELEASE toujours active
        //==========================================

        packet.destination =
            GDCC_DEST_BROADCAST;

        packet.command =
            CMD_EMERGENCY_RELEASE;

        packet.throttle =
            0;

        packet.direction =
            0;

        packet.ARU =
            0;

        packet.LIGHT_Value =
            0;
    }
}


//==================================================
// PRIORITE 3
//
// ATTENTE D'UNE NOUVELLE ACTION MANUELLE
//
// Après la fin de l'ARU, aucune commande de
// conduite n'est envoyée tant que l'utilisateur
// n'a pas effectué une nouvelle action.
//
// Une nouvelle action est détectée si :
//
//   - la locomotive sélectionnée change
//   - la vitesse change
//   - la direction change
//
//==================================================

else if (
    waitingForManualCommand
)
{
    bool newManualAction =
        (
            state.loco !=
            releaseLoco
        )
        ||
        (
            state.throttle !=
            releaseThrottle
        )
        ||
        (
            state.directionForward !=
            releaseDirectionForward
        );


    if (
        newManualAction
    )
    {
        //==========================================
        // Nouvelle action manuelle détectée
        //==========================================

        waitingForManualCommand =
            false;


        Serial.println(
            F("NOUVELLE ACTION MANUELLE - CMD_DRIVE AUTORISE")
        );


        //==========================================
        // Commande normale
        //==========================================

        packet.destination =
            state.loco;

        packet.command =
            CMD_DRIVE;

        packet.throttle =
            state.throttle;

        packet.direction =
            state.directionForward
            ? 1
            : 0;

        packet.ARU =
            0;

        packet.LIGHT_Value =
            state.light
            ? 1
            : 0;
    }
    else
    {
        //==========================================
        // Aucune nouvelle action manuelle.
        //
        // La télécommande reste silencieuse.
        //
        // Aucun CMD_DRIVE n'est envoyé.
        //
        // Un paquet RELEASE supplémentaire est
        // envoyé ici pour éviter qu'une locomotive
        // qui aurait raté le dernier RELEASE reste
        // bloquée en ARU.
        //
        //================================================

        packet.destination =
            GDCC_DEST_BROADCAST;

        packet.command =
            CMD_EMERGENCY_RELEASE;

        packet.throttle =
            0;

        packet.direction =
            0;

        packet.ARU =
            0;

        packet.LIGHT_Value =
            0;
    }
}


//==================================================
// PRIORITE 4
//
// FONCTIONNEMENT NORMAL
//
// La commande est adressée uniquement à la
// locomotive sélectionnée.
//
//==================================================

else
{
    packet.destination =
        state.loco;

    packet.command =
        CMD_DRIVE;

    packet.throttle =
        state.throttle;

    packet.direction =
        state.directionForward
        ? 1
        : 0;

    packet.ARU =
        0;

    packet.LIGHT_Value =
        state.light
        ? 1
        : 0;
}


//==================================================
// Mémorisation de l'état ARU
//
// Cette mémorisation permet de détecter la
// transition ARU actif -> ARU inactif.
//
//==================================================

previousEmergencyStop =
    state.emergencyStop;


//==================================================
// Transmission radio
//
// NO_ACK est conservé conformément au fonctionnement
// de la bibliothèque NRFLite utilisée par le projet.
//
//==================================================

radio.send(
    GDCC_RADIO_NETWORK_ID,
    &packet,
    sizeof(packet),
    NRFLite::NO_ACK
);


}
