/******************************************************************************
 *
 * GDCC
 * Gestion de l'affichage OLED
 *
 ******************************************************************************/

#include "Display.h"

#include <Wire.h>
#include <U8x8lib.h>


//======================================================
// Objet OLED
//======================================================

U8X8_SH1106_128X64_NONAME_HW_I2C display(
    U8X8_PIN_NONE
);


//======================================================
// Valeurs précédentes
//======================================================

static uint8_t lastLoco = 255;
static int lastPot = -1;
static uint16_t lastThrottle = 65535;
static bool lastDirection = false;
static bool lastLight = false;
static bool lastARU = false;


//======================================================
// Initialisation OLED
//======================================================

void Display_Init()
{
    display.begin();

    display.setPowerSave(0);

    display.setFlipMode(1);

    display.setFont(
        u8x8_font_chroma48medium8_r
    );

    display.clearDisplay();


    //==================================================
    // Ecran de démarrage
    //==================================================

    display.setCursor(1, 0);
    display.print(F("GDCC HANDSET"));

    display.setCursor(1, 2);
    display.print(F("OLED OK"));

    display.setCursor(1, 4);
    display.print(F("INITIALISATION"));

    delay(1000);

    display.clearDisplay();


    //==================================================
    // Affichage initial complet
    //==================================================

    display.setCursor(1, 0);
    display.print(F("LOCO ID: 0"));

    display.setCursor(1, 2);
    display.print(F("POT: 0"));

    display.setCursor(1, 3);
    display.print(F("SPEED: 0"));

    display.setCursor(1, 4);
    display.print(F("STOP"));

    display.setCursor(1, 5);
    display.print(F("LIGHT: OFF"));

    display.setCursor(1, 6);
    display.print(F("ARU: OK"));


    //==================================================
    // Forcer la mise à jour des valeurs
    //==================================================

    lastLoco = 255;
    lastPot = -1;
    lastThrottle = 65535;
    lastDirection = false;
    lastLight = false;
    lastARU = false;
}


//======================================================
// Affichage principal
//======================================================

void Display_Update(
    const HandsetState &state,
    int potValue
)
{
    bool displayChanged = false;


    //==================================================
    // LOCO
    //==================================================

    if (state.loco != lastLoco)
    {
        lastLoco = state.loco;
        displayChanged = true;
    }


    //==================================================
    // POTENTIOMETRE
    //==================================================

    if (potValue != lastPot)
    {
        lastPot = potValue;
        displayChanged = true;
    }


    //==================================================
    // VITESSE
    //==================================================

    if (state.throttle != lastThrottle)
    {
        lastThrottle = state.throttle;
        displayChanged = true;
    }


    //==================================================
    // DIRECTION
    //==================================================

    if (state.directionForward != lastDirection)
    {
        lastDirection = state.directionForward;
        displayChanged = true;
    }


    //==================================================
    // LIGHT
    //==================================================

    if (state.light != lastLight)
    {
        lastLight = state.light;
        displayChanged = true;
    }


    //==================================================
    // ARU
    //==================================================

    if (state.emergencyStop != lastARU)
    {
        lastARU = state.emergencyStop;
        displayChanged = true;
    }


    //==================================================
    // Rafraîchissement uniquement si nécessaire
    //==================================================

    if (!displayChanged)
    {
        return;
    }


    //==================================================
    // LOCO
    //==================================================

    display.setCursor(1, 0);

    display.print(F("LOCO ID:     "));

    display.print(state.loco);

    display.print(F("    "));


    //==================================================
    // POT
    //==================================================

    display.setCursor(1, 2);

    display.print(F("POT: "));

    display.print(potValue);

    display.print(F("    "));


    //==================================================
    // SPEED
    //==================================================

    display.setCursor(1, 3);

    display.print(F("SPEED: "));

    display.print(state.throttle);

    display.print(F("    "));


    //==================================================
    // DIRECTION
    //==================================================

    display.setCursor(1, 4);

    if (state.throttle == 0)
    {
        display.print(F("STOP         "));
    }
    else if (state.directionForward)
    {
        display.print(F("FORWARD      "));
    }
    else
    {
        display.print(F("REVERSE      "));
    }


    //==================================================
    // LIGHT
    //==================================================

    display.setCursor(1, 5);

    display.print(F("LIGHT: "));

    if (state.light)
    {
        display.print(F("ON       "));
    }
    else
    {
        display.print(F("OFF      "));
    }


    //==================================================
    // ARU
    //==================================================

    display.setCursor(1, 6);

    display.print(F("ARU: "));

    if (state.emergencyStop)
    {
        display.print(F("ACTIVE   "));
    }
    else
    {
        display.print(F("OK       "));
    }
}