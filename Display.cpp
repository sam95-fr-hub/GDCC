/******************************************************************************
 *
 * GDCC
 * Gestion de l'affichage OLED
 *
 * Version V3.1
 * U8g2 mode PAGE
 *
 ******************************************************************************/

#include "Display.h"
#include "Locomotives.h"
#include "logo.h"

#include <Wire.h>
#include <U8g2lib.h>


//======================================================
// Objet OLED
//
// Mode PAGE : faible consommation SRAM
//======================================================

U8G2_SH1106_128X64_NONAME_1_HW_I2C display(
    U8G2_R0,
    U8X8_PIN_NONE
);


//======================================================
// Etats précédents
//======================================================

static uint8_t lastLoco = 255;

static int lastPot = -1;

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


    //==================================================
    // ECRAN 1 : LOGO
    //==================================================

    display.firstPage();

    do
    {
        display.drawXBMP(
            32,
            0,
            64,
            64,
            logoBitmap
        );

    }
    while (
        display.nextPage()
    );


    delay(2000);


    //==================================================
    // ECRAN 2 : VERSION
    //==================================================

    display.firstPage();

    do
    {
        display.setFont(
            u8g2_font_6x10_tf
        );


        display.setCursor(
            20,
            20
        );

        display.print(
            F("GDCC HANDSET")
        );


        display.setCursor(
            42,
            38
        );

        display.print(
            F("V3.1")
        );


        display.setCursor(
            20,
            56
        );

        display.print(
            F("INITIALISATION")
        );

    }
    while (
        display.nextPage()
    );


    delay(2000);


    //==================================================
    // Forcer premier affichage
    //==================================================

    lastLoco = 255;

    lastPot = -1;

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
    bool needUpdate = false;


    //==================================================
    // Détection changements
    //==================================================

    if (
        state.loco != lastLoco
    )
    {
        needUpdate = true;
    }


    if (
        potValue != lastPot
    )
    {
        needUpdate = true;
    }


    if (
        state.light != lastLight
    )
    {
        needUpdate = true;
    }


    if (
        state.emergencyStop != lastARU
    )
    {
        needUpdate = true;
    }


    //==================================================
    // Aucun changement
    //==================================================

    if (!needUpdate)
    {
        return;
    }


    //==================================================
    // Récupération nom locomotive
    //==================================================

    char locoName[16];


    uint8_t locoIndex =
        state.loco - 10;


    Locomotives_GetName(
        locoIndex,
        locoName,
        sizeof(locoName)
    );


    //==================================================
    // Calcul direction
    //==================================================

    uint8_t directionState;


    if (
        potValue >= 450 &&
        potValue <= 570
    )
    {
        directionState =
            0;
    }
    else if (
        potValue < 450
    )
    {
        directionState =
            2;
    }
    else
    {
        directionState =
            1;
    }


    //==================================================
    // Calcul vitesse barre
    //==================================================

    int speed =
        0;


    if (
        potValue > 570
    )
    {
        speed =
            map(
                potValue,
                570,
                1023,
                0,
                110
            );
    }
    else if (
        potValue < 450
    )
    {
        speed =
            map(
                potValue,
                450,
                0,
                0,
                110
            );
    }


    if (
        speed < 0
    )
    {
        speed = 0;
    }


    if (
        speed > 110
    )
    {
        speed = 110;
    }


    //==================================================
    // Dessin complet écran
    //
    // Mode PAGE :
    // tout le dessin doit être répété
    // entre firstPage() et nextPage()
    //==================================================

    display.firstPage();

    do
    {
        //================================================
        // NOM LOCOMOTIVE
        //================================================

        display.setFont(
            u8g2_font_helvB12_tf
        );


        uint16_t nameWidth =
            display.getStrWidth(
                locoName
            );


        int nameX =
            (128 - nameWidth) / 2;


        if (
            nameX < 0
        )
        {
            nameX = 0;
        }


        display.setCursor(
            nameX,
            14
        );


        display.print(
            locoName
        );


        //================================================
        // ID
        //================================================

        display.setFont(
            u8g2_font_6x10_tf
        );


        display.setCursor(
            52,
            25
        );


        display.print(
            F("ID: ")
        );


        display.print(
            state.loco
        );


        //================================================
        // ARU
        //================================================

        if (
            state.emergencyStop
        )
        {
            display.setCursor(
                32,
                39
            );


            display.print(
                F("!!! ARU !!!")
            );


            display.setCursor(
                20,
                56
            );


            display.print(
                F("ARRET URGENCE")
            );
        }


        //================================================
        // FONCTIONNEMENT NORMAL
        //================================================

        else
        {
            //============================================
            // DIRECTION
            //============================================

            if (
                directionState == 0
            )
            {
                display.setCursor(
                    48,
                    38
                );


                display.print(
                    F("ARRET")
                );
            }
            else if (
                directionState == 1
            )
            {
                display.setCursor(
                    38,
                    38
                );


                display.print(
                    F("^ AVANT")
                );
            }
            else
            {
                display.setCursor(
                    34,
                    38
                );


                display.print(
                    F("v ARRIERE")
                );
            }


            //============================================
            // BARRE VITESSE
            //============================================

            display.drawFrame(
                9,
                42,
                110,
                8
            );


            if (
                speed > 0
            )
            {
                display.drawBox(
                    11,
                    44,
                    speed,
                    4
                );
            }


            //============================================
            // ICONE LUMIERE
            //============================================

            if (
                state.light
            )
            {
                display.setCursor(
                    4,
                    63
                );


                display.print(
                    F("[*]")
                );
            }
        }

    }
    while (
        display.nextPage()
    );


    //==================================================
    // Sauvegarde états
    //==================================================

    lastLoco =
        state.loco;

    lastPot =
        potValue;

    lastLight =
        state.light;

    lastARU =
        state.emergencyStop;
}