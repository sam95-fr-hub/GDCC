/******************************************************************************
 *
 * GDCC
 * Gestion des entrées de la télécommande
 *
 ******************************************************************************/

#include "Inputs.h"
#include "Config.h"
#include "Locomotives.h"


//======================================================
// Initialisation
//======================================================

void Inputs_Init()
{
    // Sélecteur locomotive
    pinMode(PIN_SELECTOR, INPUT);

    // Potentiomètre throttle
    pinMode(PIN_THROTTLE, INPUT);

    // Arrêt d'urgence
    pinMode(PIN_ESTOP, INPUT_PULLUP);

    // Éclairage
    pinMode(PIN_LIGHT, INPUT_PULLUP);

    // Batterie
    pinMode(PIN_BATTERY, INPUT);
}


//======================================================
// Mise à jour de l'état
//
// Lecture des commandes uniquement.
//
// La batterie n'est volontairement PAS lue ici.
//======================================================

void Inputs_Update(HandsetState &state)
{
    //==================================================
    // 1. SELECTEUR DE LOCOMOTIVE
    //==================================================

    int selectorADC =
        analogRead(PIN_SELECTOR);


    // Conversion ADC -> millivolts

    uint16_t selectorMillivolts =
        ((uint32_t)selectorADC * 5000UL) / 1023UL;


    // Conversion en tension pour la recherche
    // de la locomotive

    uint8_t locoIndex =
        Locomotives_GetIndex(
            selectorMillivolts / 1000.0
        );


    state.loco =
        Locomotives_GetRadioId(
            locoIndex
        );


    //==================================================
    // 2. THROTTLE
    //==================================================

    int value =
        analogRead(PIN_THROTTLE);


    // Zone morte centrale

    if (abs(value - 512) < DEAD_ZONE)
    {
        state.throttle = 0;

        state.directionForward =
            true;
    }

    else if (value > 512)
    {
        // Marche avant

        state.directionForward =
            true;


        state.throttle =
            map(
                value,
                512 + DEAD_ZONE,
                1023,
                0,
                255
            );
    }

    else
    {
        // Marche arrière

        state.directionForward =
            false;


        state.throttle =
            map(
                value,
                0,
                512 - DEAD_ZONE,
                255,
                0
            );
    }


    //==================================================
    // 3. E-STOP
    //==================================================

    static bool lastEstopButton =
        HIGH;


    bool currentEstopButton =
        digitalRead(PIN_ESTOP);


    if (lastEstopButton == HIGH &&
        currentEstopButton == LOW)
    {
        state.emergencyStop =
            !state.emergencyStop;
    }


    lastEstopButton =
        currentEstopButton;


    //==================================================
    // 4. LIGHT
    //==================================================

    static bool lastLightButton =
        HIGH;


    bool currentLightButton =
        digitalRead(PIN_LIGHT);


    if (lastLightButton == HIGH &&
        currentLightButton == LOW)
    {
        state.light =
            !state.light;
    }


    lastLightButton =
        currentLightButton;
}


//======================================================
// Lecture de la tension batterie
//
// Cette fonction est appelée uniquement au démarrage.
//
// Le pont diviseur est :
//
// R1 = 4700 ohms
// R2 = 6800 ohms
//
// La tension réelle batterie est calculée à partir
// de la tension mesurée sur l'entrée A2.
//======================================================

float Inputs_ReadBatteryVoltage()
{
    // Lecture ADC

    int batteryADC =
        analogRead(PIN_BATTERY);


    // Conversion ADC -> tension mesurée sur A2

    float voltageAtPin =
        batteryADC *
        (5.0 / 1023.0);


    // Correction du pont diviseur
    //
    // Vbatterie =
    // Vmesurée × (R1 + R2) / R2

    float batteryVoltage =
        voltageAtPin *
        (
            (BATTERY_R1 + BATTERY_R2) /
            BATTERY_R2
        );


    return batteryVoltage;
}