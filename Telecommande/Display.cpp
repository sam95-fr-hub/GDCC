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
#include "Battery.h"

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
    //==================================================
    // Initialisation écran
    //==================================================

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
    // ECRAN 3 : VERIFICATION BATTERIE
    //==================================================

    float batteryVoltage =
        Battery_ReadVoltage();


    //==================================================
    // Batterie faible
    //==================================================

    if (
        batteryVoltage < 6.4
    )
    {
        display.firstPage();

        do
        {
            //==========================================
            // Titre
            //==========================================

            display.setFont(
                u8g2_font_helvB12_tf
            );

            display.setCursor(
                16,
                16
            );

            display.print(
                F("BATTERIE")
            );


            //==========================================
            // Message
            //==========================================

            display.setCursor(
                22,
                34
            );

            display.print(
                F("FAIBLE !")
            );


            //==========================================
            // Tension
            //==========================================

            display.setFont(
                u8g2_font_6x10_tf
            );

            display.setCursor(
                36,
                47
            );

            display.print(
                batteryVoltage,
                2
            );

            display.print(
                F(" V")
            );


            //==========================================
            // Consigne
            //==========================================

            display.setCursor(
                22,
                61
            );

            display.print(
                F("CHARGER BATTERIE")
            );

        }
        while (
            display.nextPage()
        );


        //================================================
        // L'écran reste affiché
        //
        // Pour le moment, le programme reste bloqué ici.
        // Il faut recharger la batterie puis redémarrer
        // la télécommande.
        //================================================

        while (1)
        {
            delay(1000);
        }
    }


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
    // Lecture tension batterie
    //==================================================

    static float lastBatteryVoltage = -1.0;

    float batteryVoltage =
        Battery_ReadVoltage();


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
    // Mise à jour si la tension change de 0,1 V
    //==================================================

    if (
        lastBatteryVoltage < 0 ||
        abs(
            batteryVoltage -
            lastBatteryVoltage
        ) >= 0.1
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
            13
        );


        display.print(
            locoName
        );


        //================================================
        // LIGNE ID + DIRECTION
        //================================================

        display.setFont(
            u8g2_font_6x10_tf
        );


        //===============================================
        // ID à gauche
        //===============================================

        display.setCursor(
            2,
            25
        );


        display.print(
            F("ID:")
        );


        display.print(
            state.loco
        );


        //===============================================
        // Direction à droite
        //===============================================

        if (
            state.emergencyStop
        )
        {
            display.setCursor(
                70,
                25
            );

            display.print(
                F("ARU")
            );
        }
        else if (
            directionState == 0
        )
        {
            display.setCursor(
                88,
                25
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
                78,
                25
            );

            display.print(
                F("^ AVANT")
            );
        }
        else
        {
            display.setCursor(
                72,
                25
            );

            display.print(
                F("v ARRIERE")
            );
        }


        //================================================
        // ARU
        //================================================

        if (
            state.emergencyStop
        )
        {
            display.setCursor(
                32,
                40
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
            // BARRE VITESSE
            //
            // Remontée pour libérer la ligne du bas
            // pour la batterie et l'icône lumière.
            //============================================

            display.drawFrame(
                9,
                31,
                110,
                8
            );


            if (
                speed > 0
            )
            {
                display.drawBox(
                    11,
                    33,
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
                display.drawXBMP(
                    2,
                    48,
                    14,
                    14,
                    lightBitmap
                );
            }
        }


        //================================================
        // TENSION BATTERIE
        //
        // Affichage en bas à droite
        //================================================

        char batteryText[10];


        dtostrf(
            batteryVoltage,
            4,
            1,
            batteryText
        );


        uint16_t batteryWidth =
            display.getStrWidth(
                batteryText
            );


        display.setCursor(
            128 -
            batteryWidth -
            6,
            62
        );


        display.print(
            batteryText
        );


        display.print(
            F("V")
        );

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

    lastBatteryVoltage =
        batteryVoltage;
}
