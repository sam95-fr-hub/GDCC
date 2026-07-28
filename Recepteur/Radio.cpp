/******************************************************************************
*

* GDCC RECEPTEUR
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
// Initialisation du module radio
//======================================================

void Radio_Init()
{
Serial.println(
F("Initialisation NRF24L01...")
);


if (
    !radio.init(
        GDCC_RADIO_NETWORK_ID,
        PIN_RADIO_CE,
        PIN_RADIO_CSN,
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
    F("NRF24L01 OK - RESEAU GDCC")
);


Serial.print(
    F("ADRESSE RADIO RESEAU : ")
);

Serial.println(
    GDCC_RADIO_NETWORK_ID
);


Serial.print(
    F("ID LOGIQUE LOCO : ")
);

Serial.println(
    LOCO_ID
);


Serial.print(
    F("CANAL RADIO : ")
);

Serial.println(
    RADIO_CHANNEL
);


Serial.println(
    F("MODE RADIO : RECEPTION SANS ACK")
);


Serial.println(
    F("RECEPTEUR PRET")
);


}

//======================================================
// Vérification de la présence d'un paquet
//======================================================

bool Radio_Available()
{
if (
!radioInitialized
)
{
return false;
}


return (
    radio.hasData() > 0
);


}

//======================================================
// Réception d'un paquet radio
//======================================================

bool Radio_Receive(
RadioPacket &packet
)
{
if (
!radioInitialized
)
{
return false;
}


uint8_t packetLength =
    radio.hasData();


if (
    packetLength == 0
)
{
    return false;
}


//==================================================
// Vérification de la taille du paquet
//==================================================

if (
    packetLength != sizeof(RadioPacket)
)
{
    Serial.print(
        F("RX | PAQUET INVALIDE - TAILLE : ")
    );

    Serial.println(
        packetLength
    );


    radio.discardData(
        packetLength
    );


    return false;
}


//==================================================
// Lecture du paquet
//==================================================

radio.readData(
    &packet
);


//==================================================
// Vérification de la destination logique
//
// Une commande normale doit être destinée
// à cette locomotive.
//
// Les commandes globales de sécurité peuvent
// utiliser la destination broadcast.
//==================================================

bool destinationForThisLoco =
    (
        packet.destination == LOCO_ID
    );


bool broadcastPacket =
    (
        packet.destination == GDCC_DEST_BROADCAST
    );


//==================================================
// Une destination inconnue est ignorée
//==================================================

if (
    !destinationForThisLoco
    &&
    !broadcastPacket
)
{
    return false;
}


//==================================================
// Commande ARU globale
//==================================================

if (
    broadcastPacket
    &&
    packet.command == CMD_EMERGENCY_STOP
)
{
    Serial.println(
        F("RX | ARU GLOBAL")
    );

    return true;
}


//==================================================
// Sortie de l'ARU globale
//==================================================

if (
    broadcastPacket
    &&
    packet.command == CMD_EMERGENCY_RELEASE
)
{
    Serial.println(
        F("RX | FIN ARU GLOBAL")
    );

    return true;
}


//==================================================
// Les broadcasts autres que les commandes ARU
// sont ignorés.
//==================================================

if (
    broadcastPacket
)
{
    return false;
}


//==================================================
// Vérification du type de commande individuelle
//==================================================

if (
    packet.command != CMD_DRIVE
)
{
    return false;
}


//==================================================
// Affichage diagnostic
//==================================================

Serial.print(
    F("RX | DEST : ")
);

Serial.print(
    packet.destination
);


Serial.print(
    F(" | CMD : ")
);

Serial.print(
    packet.command
);


Serial.print(
    F(" | Vitesse : ")
);

Serial.print(
    packet.throttle
);


Serial.print(
    F(" | Direction : ")
);

if (
    packet.direction == 1
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
    F(" | ARU : ")
);

Serial.print(
    packet.ARU
);


Serial.print(
    F(" | LIGHT : ")
);

Serial.println(
    packet.LIGHT_Value
);


return true;


}
