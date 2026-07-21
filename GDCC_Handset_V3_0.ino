/******************************************************************************
 *
 * GDCC Handset V3.0
 *
 * Télécommande compatible avec les récepteurs V2.x
 *
 ******************************************************************************/

#include <Arduino.h>

#include "Config.h"
#include "Types.h"
#include "Inputs.h"
#include "Display.h"
#include "Radio.h"


//======================================================
// Etat de la télécommande
//======================================================

HandsetState handset;


//======================================================
// SETUP
//======================================================

void setup()
{
    Serial.begin(9600);

    delay(500);

    Serial.println();
    Serial.println(F("=============================="));
    Serial.println(F("GDCC HANDSET V3.0"));
    Serial.println(F("=============================="));


    //==================================================
    // Initialisation des entrées
    //==================================================

    Inputs_Init();

    Serial.println(F("INPUTS OK"));


    //==================================================
    // Initialisation OLED
    //==================================================

    Display_Init();

    Serial.println(F("OLED OK"));


    //==================================================
    // Initialisation radio
    //==================================================

    Radio_Init();


    //==================================================
    // Etat initial
    //==================================================

    handset.loco = 0;

    handset.throttle = 0;

    handset.directionForward = true;

    handset.light = false;

    handset.emergencyStop = false;


    Serial.println(F("SYSTEME PRET"));

    delay(1000);
}


//======================================================
// LOOP
//======================================================

void loop()
{
    //==================================================
    // 1. Lecture des entrées
    //==================================================

    Inputs_Update(handset);


    //==================================================
    // 2. Mise à jour OLED
    //==================================================

Display_Update(
    handset,
    analogRead(PIN_THROTTLE)
);


    //==================================================
    // 3. Envoi radio
    //==================================================

    Radio_Send(
        handset
    );


    //==================================================
    // 4. Moniteur série
    //==================================================

    Serial.print(F("LOCO ID : "));
    Serial.print(handset.loco);

    Serial.print(F(" | THROTTLE : "));
    Serial.print(handset.throttle);

    Serial.print(F(" | DIRECTION : "));

    if (handset.throttle == 0)
    {
        Serial.print(F("STOP"));
    }
    else if (handset.directionForward)
    {
        Serial.print(F("AVANT"));
    }
    else
    {
        Serial.print(F("ARRIERE"));
    }


    Serial.print(F(" | LIGHT : "));

    if (handset.light)
    {
        Serial.print(F("ON"));
    }
    else
    {
        Serial.print(F("OFF"));
    }


    Serial.print(F(" | ARU : "));

    if (handset.emergencyStop)
    {
        Serial.println(F("ACTIF"));
    }
    else
    {
        Serial.println(F("INACTIF"));
    }


    //==================================================
    // Rafraîchissement
    //==================================================

    delay(100);
}