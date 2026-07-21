/******************************************************************************
 *
 * GDCC
 * Gestion de l'affichage OLED
 * Version U8X8 - faible consommation SRAM
 *
 ******************************************************************************/

#include "Display.h"

#include <Wire.h>
#include <U8x8lib.h>


//======================================================
// Objet OLED SH1106
//======================================================

U8X8_SH1106_128X64_NONAME_HW_I2C display(
    U8X8_PIN_NONE
);


//======================================================
// Initialisation OLED
//======================================================

void Display_Init()
{
    display.setI2CAddress(0x3C * 2);

    display.begin();

    display.setFlipMode(1);

    display.setFont(
        u8x8_font_chroma48medium8_r
    );

    display.clear();


    //==================================================
    // Écran de démarrage
    //==================================================

    display.drawString(
        0,
        0,
        "GDCC"
    );

    display.drawString(
        0,
        2,
        "OLED OK"
    );

    display.drawString(
        0,
        4,
        "SYSTEM READY"
    );
}


//======================================================
// Affichage principal
//======================================================

void Display_Update(
    const HandsetState &state,
    int potValue
)
{
    display.clear();


    //==================================================
    // ID locomotive
    //==================================================

    display.drawString(
        0,
        0,
        "LOCO ID:"
    );

    display.setCursor(
        9,
        0
    );

    display.print(
        state.loco
    );


    //==================================================
    // Throttle
    //==================================================

    display.drawString(
        0,
        2,
        "THROTTLE:"
    );

    display.setCursor(
        10,
        2
    );

    display.print(
        potValue
    );


    //==================================================
    // Direction
    //==================================================

    display.setCursor(
        0,
        4
    );

    if (state.throttle == 0)
    {
        display.print(
            "STOP"
        );
    }
    else if (state.directionForward)
    {
        display.print(
            "FORWARD"
        );
    }
    else
    {
        display.print(
            "REVERSE"
        );
    }


    //==================================================
    // LIGHT
    //==================================================

    display.setCursor(
        0,
        5
    );

    display.print(
        "LIGHT:"
    );

    if (state.light)
    {
        display.print(
            "ON"
        );
    }
    else
    {
        display.print(
            "OFF"
        );
    }


    //==================================================
    // ARU
    //==================================================

    display.setCursor(
        0,
        6
    );

    display.print(
        "ARU:"
    );

    if (state.emergencyStop)
    {
        display.print(
            "STOP"
        );
    }
    else
    {
        display.print(
            "OK"
        );
    }
}