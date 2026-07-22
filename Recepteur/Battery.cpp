/******************************************************************************
 *
 * GDCC
 * Gestion de la batterie locomotive
 *
 * Version V3.1
 * Architecture modulaire
 *
 ******************************************************************************/

#include "Battery.h"
#include "Config.h"


//======================================================
// Initialisation de la mesure batterie
//======================================================

void Battery_Init()
{
    //==================================================
    // Configuration de l'entrée analogique
    //==================================================

    pinMode(
        PIN_BATTERY,
        INPUT
    );
}


//======================================================
// Lecture de la tension batterie
//======================================================

float Battery_ReadVoltage()
{
    //==================================================
    // Lecture brute du convertisseur analogique
    //
    // Arduino Nano :
    // ADC 10 bits
    // Valeur : 0 à 1023
    // Référence : 5 V
    //==================================================

    int rawValue = analogRead(
        PIN_BATTERY
    );


    //==================================================
    // Conversion de la valeur ADC en tension
    // présente sur l'entrée A2
    //==================================================

    float voltageAtPin =
        rawValue *
        (5.0 / 1023.0);


    //==================================================
    // Calcul de la tension réelle de la batterie
    //
    // Batterie -> R1 -> A2 -> R2 -> GND
    //
    // Vbat = Vpin * (R1 + R2) / R2
    //==================================================

    float batteryVoltage =
        voltageAtPin *
        (
            (BATTERY_R1 + BATTERY_R2)
            /
            BATTERY_R2
        );


    return batteryVoltage;
}


//======================================================
// Vérification du niveau de batterie
//======================================================

bool Battery_IsLow()
{
    //==================================================
    // Lecture de la tension batterie
    //==================================================

    float batteryVoltage =
        Battery_ReadVoltage();


    //==================================================
    // Comparaison avec le seuil minimum
    // défini dans Config.h
    //==================================================

    if (
        batteryVoltage <
        BATTERY_MIN_VOLTAGE
    )
    {
        return true;
    }


    return false;
}