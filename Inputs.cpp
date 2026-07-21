/******************************************************************************
 *
 * GDCC
 * Inputs.cpp
 *
 ******************************************************************************/

#include "Inputs.h"
#include "Config.h"

void Inputs_Init()
{
    pinMode(PIN_ESTOP, INPUT_PULLUP);
    pinMode(PIN_LIGHT, INPUT_PULLUP);
}

void Inputs_Update(HandsetState &state)
{
    //-------------------------------------------------
    // Potentiomètre
    //-------------------------------------------------

    int value = analogRead(PIN_THROTTLE);

    if (abs(value - 512) < DEAD_ZONE)
    {
        state.throttle = 0;
        state.directionForward = true;
    }
    else
    {
        if (value > 512)
        {
            state.directionForward = true;
            state.throttle = map(value, 512 + DEAD_ZONE, 1023, 0, 255);
        }
        else
        {
            state.directionForward = false;
            state.throttle = map(value, 0, 512 - DEAD_ZONE, 255, 0);
        }
    }

    //-------------------------------------------------
    // Arrêt d'urgence
    //-------------------------------------------------

    state.emergencyStop = !digitalRead(PIN_ESTOP);

    //-------------------------------------------------
    // Feux
    //-------------------------------------------------

    static bool previousLight = HIGH;

    bool current = digitalRead(PIN_LIGHT);

    if ((previousLight == HIGH) && (current == LOW))
    {
        state.light = !state.light;
    }

    previousLight = current;

    //-------------------------------------------------
    // Sélecteur loco
    //-------------------------------------------------

    int selector = analogRead(PIN_SELECTOR);

    // Valeur provisoire
    // Sera remplacée par ton tableau de correspondance

    state.loco = 1;
}