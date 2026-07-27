/******************************************************************************
*

* GDCC
* Communication radio NRF24L01
*
* Version V4.0
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
// Initialisation du module radio
//======================================================

void Radio_Init(
const HandsetState &state
)
{
//==================================================
// Initialisation du NRF24L01
//
// La télécommande possède son propre identifiant
// radio physique.
//
// Les récepteurs utilisent tous la même adresse
// radio physique de réseau :
//
//     GDCC_RADIO_NETWORK_ID
//
// L'ID logique de la locomotive sélectionnée n'est
// PAS utilisé comme adresse NRF24L01.
//
// Il est placé dans :
//
//     packet.destination
//
// Le canal radio est configuré directement dans
// radio.init(), conformément à l'API réelle
// de NRFLite.
//==================================================

```
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
// Etat radio initialisé
//==================================================

radioInitialized = true;


//==================================================
// Diagnostic
//==================================================

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
```

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

```
if (
    !radioInitialized
)
{
    return;
}


//==================================================
// Création du paquet GDCC V4.0
//==================================================

RadioPacket packet;


//==================================================
// Destination logique
//
// L'ID de la locomotive est transporté directement
// dans la trame.
//
// Il ne correspond PAS à l'adresse radio physique.
//==================================================

packet.destination =
    state.loco;


//==================================================
// Type de commande
//
// V4.0 :
// uniquement CMD_DRIVE.
//==================================================

packet.command =
    CMD_DRIVE;


//==================================================
// Vitesse
//
//   0   = arrêt
//   255 = vitesse maximale
//==================================================

packet.throttle =
    state.throttle;


//==================================================
// Direction
//
//   0 = arrière
//   1 = avant
//==================================================

packet.direction =
    state.directionForward
    ? 1
    : 0;


//==================================================
// Arrêt d'urgence
//
//   0 = normal
//   1 = actif
//
// Le champ est transmis dans la trame V4.0.
//==================================================

packet.ARU =
    state.emergencyStop
    ? 1
    : 0;


//==================================================
// Eclairage
//
//   0 = éteint
//   1 = allumé
//==================================================

packet.LIGHT_Value =
    state.light
    ? 1
    : 0;


//==================================================
// Transmission radio V4.0
//
// Destination physique :
//
//     GDCC_RADIO_NETWORK_ID
//
// Destination logique :
//
//     packet.destination
//
// Mode de transmission :
//
//     NRFLite::NO_ACK
//
// Le NRF24L01 transmet le paquet une seule fois.
//
// Aucun ACK n'est demandé au récepteur.
//
// Aucune retransmission automatique n'est effectuée
// en cas de perte du paquet.
//==================================================

radio.send(
    GDCC_RADIO_NETWORK_ID,
    &packet,
    sizeof(packet),
    NRFLite::NO_ACK
);
```

}
