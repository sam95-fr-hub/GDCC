/******************************************************************************
 *
 * GDCC
 * Battery.h
 *
 * Gestion de la mesure batterie de la télécommande
 *
 ******************************************************************************/

#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

void Battery_Init();
float Battery_ReadVoltage();

#endif