/******************************************************************************
 *
 * GDCC
 * Gestion des locomotives
 *
 * Version V3.1
 *
 ******************************************************************************/

#include "Locomotives.h"

#include <avr/pgmspace.h>
#include <string.h>


//======================================================
// Tableau des locomotives
//
// Stocké entièrement en mémoire Flash.
//======================================================

const Locomotive locomotives[
    LOCOMOTIVE_COUNT
]
PROGMEM =
{
    { 0.10, 10 },
    { 0.54, 11 },
    { 1.02, 12 },
    { 1.47, 13 },
    { 1.92, 14 },
    { 2.27, 15 },
    { 2.75, 16 },
    { 3.21, 17 },
    { 3.60, 18 },
    { 4.03, 19 },
    { 4.46, 20 },
    { 4.89, 21 }
};


//======================================================
// Noms des locomotives
//
// Stockés en mémoire Flash.
//======================================================

const char locoName0[] PROGMEM = "RUSTY";
const char locoName1[] PROGMEM = "GEMINI";
const char locoName2[] PROGMEM = "TOMY";
const char locoName3[] PROGMEM = "THUNDER";
const char locoName4[] PROGMEM = "STORM";
const char locoName5[] PROGMEM = "BLAZE";
const char locoName6[] PROGMEM = "COMET";
const char locoName7[] PROGMEM = "VULCAN";
const char locoName8[] PROGMEM = "ZEPHYR";
const char locoName9[] PROGMEM = "AURORA";
const char locoName10[] PROGMEM = "TITAN";
const char locoName11[] PROGMEM = "NEBULA";


//======================================================
// Tableau des pointeurs vers les noms
//
// Stocké en mémoire Flash.
//======================================================

const char* const locoNames[
    LOCOMOTIVE_COUNT
]
PROGMEM =
{
    locoName0,
    locoName1,
    locoName2,
    locoName3,
    locoName4,
    locoName5,
    locoName6,
    locoName7,
    locoName8,
    locoName9,
    locoName10,
    locoName11
};


//======================================================
// Recherche de la locomotive la plus proche
//======================================================

uint8_t Locomotives_GetIndex(
    float voltage
)
{
    uint8_t bestIndex = 0;

    float referenceVoltage;


    //==================================================
    // Première locomotive
    //==================================================

    memcpy_P(
        &referenceVoltage,
        &locomotives[0].referenceVoltage,
        sizeof(float)
    );


    float bestDifference =
        voltage - referenceVoltage;


    if (bestDifference < 0)
    {
        bestDifference =
            -bestDifference;
    }


    //==================================================
    // Recherche
    //==================================================

    for (
        uint8_t i = 1;
        i < LOCOMOTIVE_COUNT;
        i++
    )
    {
        memcpy_P(
            &referenceVoltage,
            &locomotives[i].referenceVoltage,
            sizeof(float)
        );


        float difference =
            voltage - referenceVoltage;


        if (difference < 0)
        {
            difference =
                -difference;
        }


        if (difference < bestDifference)
        {
            bestDifference =
                difference;

            bestIndex =
                i;
        }
    }


    return bestIndex;
}


//======================================================
// Retourne l'identifiant radio
//======================================================

uint8_t Locomotives_GetRadioId(
    uint8_t index
)
{
    if (
        index >= LOCOMOTIVE_COUNT
    )
    {
        return 0;
    }


    uint8_t radioId;


    memcpy_P(
        &radioId,
        &locomotives[index].radioId,
        sizeof(uint8_t)
    );


    return radioId;
}


//======================================================
// Retourne le nom de la locomotive
//======================================================

void Locomotives_GetName(
    uint8_t index,
    char* buffer,
    uint8_t bufferSize
)
{
    if (
        buffer == nullptr ||
        bufferSize == 0
    )
    {
        return;
    }


    //==================================================
    // Buffer vide par défaut
    //==================================================

    buffer[0] =
        '\0';


    //==================================================
    // Vérification index
    //==================================================

    if (
        index >= LOCOMOTIVE_COUNT
    )
    {
        return;
    }


    //==================================================
    // Lecture du pointeur depuis la Flash
    //==================================================

    const char* namePointer =
        (const char*)pgm_read_word(
            &locoNames[index]
        );


    //==================================================
    // Copie du nom vers le buffer
    //==================================================

    strncpy_P(
        buffer,
        namePointer,
        bufferSize - 1
    );


    //==================================================
    // Sécurité fin de chaîne
    //==================================================

    buffer[
        bufferSize - 1
    ] =
        '\0';
}
