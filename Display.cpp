/******************************************************************************
 *
 * GDCC
 * Gestion de l'affichage OLED
 *
 ******************************************************************************/

#include "Display.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>


//======================================================
// Objet OLED
//======================================================

Adafruit_SH1106 display(-1);


//======================================================
// Initialisation OLED
//======================================================

void Display_Init()
{
    display.begin(
        SH1106_SWITCHCAPVCC,
        0x3C
    );

    display.setRotation(2);

    display.clearDisplay();

    display.setTextColor(WHITE);

    //==================================================
    // Ecran de démarrage
    //==================================================

    display.setTextSize(2);

    display.setCursor(0, 0);

    display.print(F("GDCC"));

    display.setTextSize(1);

    display.setCursor(0, 25);

    display.print(F("HANDSET V3"));

    display.setCursor(0, 40);

    display.print(F("DISPLAY OK"));

    display.display();
}


//======================================================
// Affichage principal
//======================================================

void Display_Update(
    const HandsetState &state,
    int potValue
)
{
    display.clearDisplay();


    //==================================================
    // RADIO ID
    //==================================================

    display.setTextSize(2);

    display.setCursor(0, 0);

    display.print(F("LOCO "));

    display.print(state.loco);


    //==================================================
    // THROTTLE
    //==================================================

    display.setTextSize(1);

    display.setCursor(0, 22);

    display.print(F("Throttle: "));

    display.print(potValue);


    //==================================================
    // DIRECTION
    //==================================================

    display.setCursor(0, 34);

    if (state.throttle == 0)
    {
        display.print(F("STOP"));
    }
    else if (state.directionForward)
    {
        display.print(F("FORWARD"));
    }
    else
    {
        display.print(F("REVERSE"));
    }


    //==================================================
    // LIGHT
    //==================================================

    display.setCursor(0, 46);

    display.print(F("LIGHT: "));

    if (state.light)
    {
        display.print(F("ON"));
    }
    else
    {
        display.print(F("OFF"));
    }


    //==================================================
    // ARRET D'URGENCE
    //==================================================

    display.setCursor(0, 58);

    display.print(F("ARU: "));

    if (state.emergencyStop)
    {
        display.print(F("STOP"));
    }
    else
    {
        display.print(F("OK"));
    }


    //==================================================
    // Mise à jour OLED
    //==================================================

    display.display();
}