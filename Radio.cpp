/******************************************************************************
 *
 * GDCC
 * Communication radio NRF24L01
 *
 * Compatible avec les récepteurs V2.x
 *
 ******************************************************************************/

#include "Radio.h"
#include "Config.h"

#include <SPI.h>
#include <RF24.h>


//======================================================
// Objet NRF24L01
//======================================================

RF24 radio(
    PIN_NRF_CE,
    PIN_NRF_CSN
);


//======================================================
// Adresse radio
//
// Le récepteur V2.x doit utiliser la même adresse
// que celle définie dans son programme.
//
// Adresse conservée pour compatibilité.
//======================================================

const byte RADIO_ADDRESS[6] = "GDCC";


//======================================================
// Initialisation radio
//======================================================

void Radio_Init()
{
    if (!radio.begin())
    {
        Serial.println("ERREUR NRF24L01");
        return;
    }


    // Canal radio
    radio.setChannel(
        RADIO_CHANNEL
    );


    // Puissance d'émission
    radio.setPALevel(
        RF24_PA_LOW
    );


    // Débit
    radio.setDataRate(
        RF24_250KBPS
    );


    // Adresse du récepteur
    radio.openWritingPipe(
        RADIO_ADDRESS
    );


    // Pas d'accusé de réception
    // pour conserver le comportement
    // simple du système V2.x

    radio.setAutoAck(
        false
    );


    // Mode émission
    radio.stopListening();


    Serial.println("NRF24L01 OK");
}


//======================================================
// Envoi du paquet radio
//======================================================

void Radio_Send(
    const HandsetState &state
)
{
    RadioPacket packet;


    //==================================================
    // THROTTLE
    //==================================================

    packet.POT_Value =
        state.throttle;


    //==================================================
    // ARU
    //==================================================

    if (state.emergencyStop)
    {
        packet.ARU = 1;
    }
    else
    {
        packet.ARU = 0;
    }


    //==================================================
    // LIGHT
    //==================================================

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

    radio.write(
        &packet,
        sizeof(packet)
    );
}