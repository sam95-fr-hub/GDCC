/******************************************************************************
*

* GDCC RECEPTEUR
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

void Radio_Init()
{
Serial.println(
F("Initialisation NRF24L01...")
);


//==================================================
// Initialisation du NRF24L01
//
// V4.0 :
//
// Tous les récepteurs du réseau GDCC utilisent
// la même adresse radio physique.
//
// Cette adresse est définie par :
//
//     GDCC_RADIO_NETWORK_ID
//
// L'identité logique de la locomotive est définie
// séparément par :
//
//     LOCO_ID
//
// Le canal radio est défini par :
//
//     RADIO_CHANNEL
//
// IMPORTANT :
//
// Le NRF24L01 ne permet pas ici d'avoir plusieurs
// récepteurs avec exactement la même adresse sur
// le même canal et de les distinguer physiquement.
//
// Ils reçoivent donc tous les paquets transmis
// sur l'adresse commune.
//
// Le filtrage logique est ensuite effectué grâce
// au champ :
//
//     packet.destination
//==================================================

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


    //================================================
    // Blocage en cas d'erreur radio
    //================================================

    while (1)
    {
        // Sécurité :
        // le récepteur ne démarre pas
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
    F("MODE TRANSMISSION : NO_ACK")
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
//==================================================
// Vérification de l'initialisation
//==================================================


if (
    !radioInitialized
)
{
    return false;
}


//==================================================
// NRFLite vérifie le FIFO RX du NRF24L01.
//
// hasData() retourne la longueur du paquet reçu
// sur le pipe de données normal.
//
// Le paquet n'est pas encore retiré du FIFO.
//==================================================

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
//==================================================
// Vérification de l'initialisation
//==================================================


if (
    !radioInitialized
)
{
    return false;
}


//==================================================
// Vérification de la présence d'un paquet
//==================================================

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
//
// La trame GDCC V4.0 doit faire exactement :
//
//     6 octets
//
// Si une trame d'une autre taille est reçue,
// elle est supprimée du FIFO RX et ignorée.
//
// Cette vérification est importante car NRFLite
// utilise des paquets de longueur dynamique.
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
// Filtrage par destination logique
//
// V4.0 :
//
// Seule la locomotive dont l'ID correspond à :
//
//     LOCO_ID
//
// traite la commande.
//
// Le broadcast 255 est réservé aux futures
// versions du protocole.
//==================================================

if (
    packet.destination != LOCO_ID
)
{
    //================================================
    // Paquet destiné à une autre locomotive.
    //
    // Il a déjà été retiré du FIFO RX par
    // radio.readData().
    //
    // On l'ignore simplement.
    //================================================

    return false;
}


//==================================================
// Vérification du type de commande
//
// V4.0 :
//
// Seule CMD_DRIVE est actuellement traitée.
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


//==================================================
// Paquet valide et destiné à cette locomotive
//==================================================

return true;


}
