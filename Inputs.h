/******************************************************************************
 *
 * GDCC
 * Inputs.h
 *
 * Gestion de toutes les entrées utilisateur
 *
 ******************************************************************************/

#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "Types.h"

void Inputs_Init();
void Inputs_Update(HandsetState &state);

#endif