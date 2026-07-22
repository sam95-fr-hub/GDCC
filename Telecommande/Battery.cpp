/******************************************************************************
 *
 * GDCC
 * Mesure de la batterie
 *
 ******************************************************************************/

#include "Battery.h"
#include "Config.h"

//======================================================
// Initialisation
//======================================================

void Battery_Init()
{
    pinMode(PIN_BATTERY, INPUT);
}

//======================================================
// Lecture de la tension batterie
//======================================================

float Battery_ReadVoltage()
{
    long total = 0;

    // Moyenne de 16 mesures
    for (int i = 0; i < 16; i++)
    {
        total += analogRead(PIN_BATTERY);
    }

    float sensorValue = total / 16.0;

    // Conversion ADC vers tension Arduino
    float voltage = sensorValue * (5.0 / 1023.0);

    // Correction du pont diviseur
    voltage *= (BATTERY_R1 + BATTERY_R2) / BATTERY_R2;

    return voltage;
}