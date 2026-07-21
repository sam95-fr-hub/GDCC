/******************************************************************************
 *
 * GDCC Handset V3.0
 * Télécommande compatible avec les récepteurs V2.x
 *
 * Fonctionnalités :
 * - Sélecteur 12 positions
 * - Sélection par Radio ID
 * - Throttle
 * - Marche avant / arrière
 * - LIGHT
 * - ARU
 * - OLED
 *
 * Temporairement supprimé :
 * - Lecture batterie
 * - Noms des locomotives
 *
 ******************************************************************************/

#include <Arduino.h>

#include "Config.h"
#include "Types.h"
#include "Inputs.h"
#include "Locomotives.h"
#include "Display.h"


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
    Serial.println("==============================");
    Serial.println("GDCC HANDSET V3.0");
    Serial.println("==============================");


    //==================================================
    // Initialisation des entrées
    //==================================================

    Inputs_Init();

    Serial.println("INPUTS OK");


    //==================================================
    // Initialisation OLED
    //==================================================

    Display_Init();

    Serial.println("OLED OK");


    //==================================================
    // Etat initial
    //==================================================

    handset.loco =
        Locomotives_GetRadioId(0);

    handset.throttle = 0;

    handset.directionForward = true;

    handset.light = false;

    handset.emergencyStop = false;


    Serial.println("SYSTEME PRET");

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
    // 2. Lecture brute du throttle
    //==================================================

    int potValue =
        analogRead(PIN_THROTTLE);


    //==================================================
    // 3. Mise à jour OLED
    //==================================================

    Display_Update(
        handset,
        potValue
    );


    //==================================================
    // 4. Moniteur série
    //==================================================

    Serial.print("LOCO ID : ");

    Serial.print(handset.loco);


    Serial.print(" | THROTTLE : ");

    Serial.print(potValue);


    Serial.print(" | DIRECTION : ");


    if (handset.throttle == 0)
    {
        Serial.print("STOP");
    }
    else if (handset.directionForward)
    {
        Serial.print("AVANT");
    }
    else
    {
        Serial.print("ARRIERE");
    }


    Serial.print(" | LIGHT : ");


    if (handset.light)
    {
        Serial.print("ON");
    }
    else
    {
        Serial.print("OFF");
    }


    Serial.print(" | ARU : ");


    if (handset.emergencyStop)
    {
        Serial.println("ACTIF");
    }
    else
    {
        Serial.println("INACTIF");
    }


    //==================================================
    // Rafraîchissement
    //==================================================

    delay(500);
}