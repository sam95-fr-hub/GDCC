/******************************************************************************
 *
 * GDCC
 * Battery.cpp
 *
 ******************************************************************************/

#include "Battery.h"
#include "Config.h"

#define BATTERY_SAMPLES 16

void Battery_Init()
{
    pinMode(PIN_BATTERY, INPUT);
}

float Battery_ReadVoltage()
{
    long total = 0;

    // Moyenne de plusieurs mesures pour réduire le bruit
    for (uint8_t i = 0; i < BATTERY_SAMPLES; i++)
    {
        total += analogRead(PIN_BATTERY);
        delayMicroseconds(200);
    }

    float adc = total / (float)BATTERY_SAMPLES;

    // Référence ADC (UNO)
    float voltage = adc * (5.0 / 1023.0);

    // Correction du pont diviseur
    voltage *= (BATTERY_R1 + BATTERY_R2) / BATTERY_R2;

    return voltage;
}
}