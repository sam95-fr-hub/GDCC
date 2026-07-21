/******************************************************************************
 *
 * GDCC
 * Gestion des locomotives
 *
 ******************************************************************************/

#ifndef LOCOMOTIVES_H
#define LOCOMOTIVES_H

#include <Arduino.h>
#include <avr/pgmspace.h>

//======================================================
// Nombre de locomotives
//======================================================

#define LOCOMOTIVE_COUNT 12


//======================================================
// Structure locomotive
//
// Les noms sont stockés séparément en PROGMEM.
//======================================================

struct Locomotive
{
    float referenceVoltage;

    uint8_t radioId;
};


//======================================================
// Tableau des locomotives
//======================================================

extern const Locomotive locomotives[LOCOMOTIVE_COUNT] PROGMEM;


//======================================================
// Fonctions
//======================================================

uint8_t Locomotives_GetIndex(
    float voltage);


uint8_t Locomotives_GetRadioId(
    uint8_t index);


void Locomotives_GetName(
    uint8_t index,
    char* buffer,
    uint8_t bufferSize);


#endif