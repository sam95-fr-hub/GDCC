/******************************************************************************
 *
 * GDCC
 * Gestion de la communication radio NRF24L01
 *
 ******************************************************************************/

#include "Radio.h"
#include "Config.h"
#include <NRFLite.h>

static NRFLite radio;

//======================================================
// Initialisation
//======================================================

bool Radio_Init()
{
    if (!radio.init(RADIO_ID, PIN_NRF_CE, PIN_NRF_CSN))
    {
        return false;
    }

    return true;
}


//======================================================
// Envoi des données
//======================================================

bool Radio_Send(uint8_t destinationId, const RadioPacket &packet)
{
    return radio.send(
        destinationId,
        (void*)&packet,
        sizeof(packet)
    );
}