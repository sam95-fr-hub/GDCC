/******************************************************************************
 *
 * GDCC
 * Gestion de l'affichage OLED
 *
 ******************************************************************************/

#include "Display.h"
#include "Config.h"

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
    // Initialisation de l'écran SH1106
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);

    // Rotation utilisée sur la télécommande
    display.setRotation(2);

    // Effacement
    display.clearDisplay();

    // Couleur du texte
    display.setTextColor(WHITE);

    // Affichage du premier écran
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("GDCC");

    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print("OLED TEST");

    display.setCursor(0, 40);
    display.print("DISPLAY OK");

    // Envoi vers l'écran
    display.display();
}


//======================================================
// Affichage principal
//======================================================

void Display_Update(
    const HandsetState &state,
    const char* locoName,
    int potValue
)
{
    display.clearDisplay();

    // Nom de la locomotive
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(locoName);

    // Radio ID
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print("Radio ID: ");
    display.print(state.loco);

    // Throttle
    display.setCursor(0, 32);
    display.print("Throttle: ");
    display.print(potValue);

    // Direction
    display.setCursor(0, 42);

    if (state.throttle == 0)
    {
        display.print("STOP");
    }
    else if (state.directionForward)
    {
        display.print("FORWARD");
    }
    else
    {
        display.print("REVERSE");
    }

    // Etats LIGHT et ARU
    display.setCursor(0, 54);

    display.print("L:");
    display.print(state.light ? "ON " : "OFF");

    display.print(" A:");
    display.print(state.emergencyStop ? "STOP" : "OK");

    // Mise à jour OLED
    display.display();
}