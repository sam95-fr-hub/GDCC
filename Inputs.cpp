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
//======================================================

void Inputs_Update(HandsetState &state)
{
    //==================================================
    // 1. SELECTEUR DE LOCOMOTIVE
    //==================================================

    int selectorADC =
        analogRead(PIN_SELECTOR);


    float selectorVoltage =
        selectorADC *
        (5.0 / 1023.0);


    uint8_t locoIndex =
        Locomotives_GetIndex(
            selectorVoltage);


    state.loco =
        Locomotives_GetRadioId(
            locoIndex);


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
            map(value,
                512 + DEAD_ZONE,
                1023,
                0,
                255);
    }

    else
    {
        // Marche arrière

        state.directionForward =
            false;


        state.throttle =
            map(value,
                0,
                512 - DEAD_ZONE,
                255,
                0);
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


    //==================================================
    // 5. BATTERIE
    //==================================================

    int batteryADC =
        analogRead(PIN_BATTERY);


    // Tension réellement présente sur A2

    float batteryPinVoltage =
        batteryADC *
        (5.0 / 1023.0);


    // Reconstitution de la tension batterie
    // avec le pont diviseur :
    //
    // Batterie ---- R1 ---- A2 ---- R2 ---- GND
    //
    // R1 = 4700 ohms
    // R2 = 6800 ohms

    state.batteryVoltage =
        batteryPinVoltage *
        (BATTERY_R1 + BATTERY_R2) /
        BATTERY_R2;
}