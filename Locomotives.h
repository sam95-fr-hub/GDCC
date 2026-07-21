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
// Contient uniquement les informations nécessaires
// à la sélection de la locomotive.
//
// Les données sont stockées en mémoire Flash.
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

// Recherche la locomotive correspondant
// à la tension du sélecteur

uint8_t Locomotives_GetIndex(
    float voltage);


// Retourne le Radio ID de la locomotive

uint8_t Locomotives_GetRadioId(
    uint8_t index);


#endif