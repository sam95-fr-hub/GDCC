/******************************************************************************
 *
 * GDCC Handset V3.0
 * TEST OLED + INPUTS
 *
 * Test :
 * - Sélecteur 12 positions
 * - Throttle
 * - LIGHT
 * - ARU
 * - Lecture batterie au démarrage uniquement
 * - OLED dynamique
 *
 * Pas de :
 * - NRF24L01
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
// Tension batterie mesurée au démarrage
//======================================================

float startupBatteryVoltage = 0.0;


//======================================================
// SETUP
//======================================================

void setup()
{
    Serial.begin(9600);

    delay(500);


    //==================================================
    // Message de démarrage
    //==================================================

    Serial.println();
    Serial.println("==============================");
    Serial.println("GDCC HANDSET V3.0");
    Serial.println("TEST OLED + INPUTS");
    Serial.println("==============================");


    //==================================================
    // Initialisation des entrées
    //==================================================

    Inputs_Init();

    Serial.println("INPUTS OK");


    //==================================================
    // Lecture batterie
    //==================================================

    Serial.println();
    Serial.println("------------------------------");
    Serial.println("LECTURE BATTERIE");
    Serial.println("------------------------------");


    startupBatteryVoltage =
        Inputs_ReadBatteryVoltage();


    Serial.print("BATTERIE : ");
    Serial.print(
        startupBatteryVoltage,
        2
    );
    Serial.println(" V");


    Serial.println("LECTURE BATTERIE OK");


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


    //==================================================
    // Message système prêt
    //==================================================

    Serial.println();
    Serial.println("------------------------------");
    Serial.println("SYSTEME PRET");
    Serial.println("------------------------------");


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
    // 2. Recherche de l'index de la locomotive
    //==================================================

    uint8_t locoIndex = 0;

    for (uint8_t i = 0;
         i < LOCOMOTIVE_COUNT;
         i++)
    {
        if (Locomotives_GetRadioId(i) == handset.loco)
        {
            locoIndex = i;
            break;
        }
    }


    //==================================================
    // 3. Récupération du nom
    //==================================================

    char locoName[20];

    Locomotives_GetName(
        locoIndex,
        locoName,
        sizeof(locoName)
    );


    //==================================================
    // 4. Lecture brute du throttle
    //==================================================

    int potValue =
        analogRead(PIN_THROTTLE);


    //==================================================
    // 5. Mise à jour OLED
    //==================================================

    Display_Update(
        handset,
        locoName,
        potValue
    );


    //==================================================
    // 6. Moniteur série
    //==================================================

    Serial.print("LOCO : ");
    Serial.print(locoName);

    Serial.print(" | ID : ");
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
    // 7. Rafraîchissement lent
    //==================================================

    delay(500);
}