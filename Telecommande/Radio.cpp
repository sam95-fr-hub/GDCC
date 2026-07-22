/******************************************************************************
 *
 * GDCC
 * Communication radio NRF24L01
 *
 * Compatible avec les récepteurs V2.x utilisant NRFLite
 *
 ******************************************************************************/

#include "Radio.h"
#include "Config.h"

#include <SPI.h>
#include <NRFLite.h>


//======================================================
// Objet radio
//======================================================

NRFLite radio;


//======================================================
// Initialisation radio
//======================================================

void Radio_Init()
{
    // La radio sera initialisée dans Radio_Send()
    // car le RADIO_ID dépend de la locomotive sélectionnée.

    Serial.println(F("RADIO NRFLite PRETE"));
}


//======================================================
// Envoi du paquet radio
//======================================================

void Radio_Send(
    const HandsetState &state
)
{
    static uint8_t lastRadioId = 0;


    //==================================================
    // Initialisation / changement de locomotive
    //==================================================

    if (state.loco != lastRadioId)
    {
        if (!radio.init(
                state.loco,
                PIN_NRF_CE,
                PIN_NRF_CSN))
        {
            Serial.println(F("ERREUR NRF24L01"));
            return;
        }

        lastRadioId =
            state.loco;

        Serial.print(F("RADIO LOCO ID : "));
        Serial.println(state.loco);
    }


    //==================================================
    // Création du paquet
    //==================================================

    RadioPacket packet;


    // Valeur brute du potentiomètre
    // Compatible avec le récepteur V2.x

    packet.POT_Value =
        state.potValue;


    // ARU

    if (state.emergencyStop)
    {
        packet.ARU = 1;
    }
    else
    {
        packet.ARU = 0;
    }


    // LIGHT

    if (state.light)
    {
        packet.LIGHT_Value = 1;
    }
    else
    {
        packet.LIGHT_Value = 0;
    }


    //==================================================
    // Envoi
    //==================================================

    radio.send(
        state.loco,
        &packet,
        sizeof(packet)
    );
}