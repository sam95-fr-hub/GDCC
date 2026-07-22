```cpp
/******************************************************************************
 *
 * GDCC
 * Gestion des entrées de la télécommande
 *
 * Version V3.1
 * Architecture modulaire
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
    pinMode(
        PIN_SELECTOR,
        INPUT
    );

    pinMode(
        PIN_THROTTLE,
        INPUT
    );

    pinMode(
        PIN_ESTOP,
        INPUT_PULLUP
    );

    pinMode(
        PIN_LIGHT,
        INPUT_PULLUP
    );
}


//======================================================
// Mise à jour de l'état
//======================================================

void Inputs_Update(
    HandsetState &state
)
{
    //==================================================
    // 1. SELECTEUR DE LOCOMOTIVE
    //==================================================

    int selectorADC =
        analogRead(
            PIN_SELECTOR
        );


    uint16_t selectorMillivolts =
        (
            (uint32_t)selectorADC *
            5000UL
        )
        /
        1023UL;


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
        analogRead(
            PIN_THROTTLE
        );


    //==================================================
    // Valeur brute du potentiomètre
    //
    // Cette valeur reste disponible localement
    // pour l'affichage ou le diagnostic.
    //
    // IMPORTANT :
    // Elle n'est plus transmise par radio.
    //
    // La transmission utilise désormais :
    //
    //   state.throttle         -> 0 à 255
    //   state.directionForward -> avant / arrière
    //==================================================

    state.potValue =
        value;


    //==================================================
    // Zone morte centrale
    //==================================================

    if (
        abs(value - 512) <
        DEAD_ZONE
    )
    {
        //==============================================
        // Arrêt
        //==============================================

        state.throttle =
            0;


        //==============================================
        // Direction par défaut
        //
        // La direction est sans importance lorsque
        // throttle = 0.
        //
        // On conserve AVANT comme valeur par défaut.
        //==============================================

        state.directionForward =
            true;
    }


    //==================================================
    // Marche avant
    //==================================================

    else if (
        value >
        512
    )
    {
        state.directionForward =
            true;


        //==============================================
        // Conversion :
        //
        // 512 + DEAD_ZONE -> 0
        // 1023            -> 255
        //==============================================

        long throttle =
            map(
                value,
                512 + DEAD_ZONE,
                1023,
                0,
                255
            );


        //==============================================
        // Sécurité :
        // limitation à 0-255
        //==============================================

        throttle =
            constrain(
                throttle,
                0,
                255
            );


        state.throttle =
            (uint8_t) throttle;
    }


    //==================================================
    // Marche arrière
    //==================================================

    else
    {
        state.directionForward =
            false;


        //==============================================
        // Conversion :
        //
        // 0                 -> 255
        // 512 - DEAD_ZONE   -> 0
        //==============================================

        long throttle =
            map(
                value,
                0,
                512 - DEAD_ZONE,
                255,
                0
            );


        //==============================================
        // Sécurité :
        // limitation à 0-255
        //==============================================

        throttle =
            constrain(
                throttle,
                0,
                255
            );


        state.throttle =
            (uint8_t) throttle;
    }


    //==================================================
    // 3. E-STOP
    //==================================================

    static bool lastEstopButton =
        HIGH;


    bool currentEstopButton =
        digitalRead(
            PIN_ESTOP
        );


    //==================================================
    // Détection d'un nouvel appui
    //
    // Le bouton est relié à la masse.
    //
    // HIGH -> LOW = nouvel appui
    //
    // Chaque appui inverse l'état de l'ARU.
    //==================================================

    if (
        lastEstopButton == HIGH &&
        currentEstopButton == LOW
    )
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
        digitalRead(
            PIN_LIGHT
        );


    //==================================================
    // Détection d'un nouvel appui
    //
    // Le bouton est relié à la masse.
    //
    // HIGH -> LOW = nouvel appui
    //
    // Chaque appui inverse l'état de l'éclairage.
    //==================================================

    if (
        lastLightButton == HIGH &&
        currentLightButton == LOW
    )
    {
        state.light =
            !state.light;


        Serial.print(
            F("LIGHT CHANGE : ")
        );


        if (
            state.light
        )
        {
            Serial.println(
                F("ON")
            );
        }
        else
        {
            Serial.println(
                F("OFF")
            );
        }
    }


    lastLightButton =
        currentLightButton;
}
```
