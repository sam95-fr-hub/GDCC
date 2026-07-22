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
// Initialisation du module radio
//======================================================

void Radio_Init()
{
    Serial.println(
        F("Initialisation NRF24L01...")
    );


    //==================================================
    // Initialisation du NRF24L01
    //==================================================

    if (
        !radio.init(
            RADIO_ID,
            PIN_RADIO_CE,
            PIN_RADIO_CSN
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


    Serial.print(
        F("NRF24L01 OK - RADIO ID : ")
    );

    Serial.println(
        RADIO_ID
    );
}


//======================================================
// Vérification de la présence d'un paquet
//======================================================

bool Radio_Available()
{
    return radio.hasData();
}


//======================================================
// Réception d'un paquet radio
//======================================================

bool Radio_Receive(
    RadioPacket &packet
)
{
    //==================================================
    // Vérification
    //==================================================

    if (
        !radio.hasData()
    )
    {
        return false;
    }


    //==================================================
    // Lecture du paquet
    //==================================================

    radio.readData(
        &packet
    );


    //==================================================
    // Affichage diagnostic
    //==================================================

    Serial.print(
        F("RX | Vitesse : ")
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