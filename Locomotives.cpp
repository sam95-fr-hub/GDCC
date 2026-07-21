/******************************************************************************
 *
 * GDCC
 * Gestion des locomotives
 *
 ******************************************************************************/

#include "Locomotives.h"


//======================================================
// Tableau des locomotives
//
// Stocké en mémoire Flash.
//======================================================

const Locomotive locomotives[LOCOMOTIVE_COUNT]
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
// Recherche de la locomotive la plus proche
//======================================================

uint8_t Locomotives_GetIndex(
    float voltage)
{
    uint8_t bestIndex = 0;

    float referenceVoltage;

    // Lecture de la première tension depuis la Flash

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


    // Recherche de la tension la plus proche

    for (uint8_t i = 1;
         i < LOCOMOTIVE_COUNT;
         i++)
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
    uint8_t index)
{
    if (index >= LOCOMOTIVE_COUNT)
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