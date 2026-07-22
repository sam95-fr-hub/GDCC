/******************************************************************************
 *
 * GDCC
 * Récepteur locomotive
 *
 * Version V3.1
 * Architecture modulaire
 *
 * Matériel :
 *   Arduino Nano
 *   NRF24L01
 *   L298N
 *
 ******************************************************************************/

#include "Config.h"
#include "Types.h"

#include "Radio.h"
#include "Motor.h"
#include "Functions.h"
#include "Battery.h"


//======================================================
// Etat reçu par radio
//======================================================

RadioPacket radioPacket;


//======================================================
// Gestion du timeout radio
//======================================================

// Moment de réception du dernier paquet radio valide
unsigned long lastRadioPacketTime = 0;


// Etat actuel de la liaison radio
// false = liaison perdue
// true  = liaison active
bool radioConnected = false;


//======================================================
// Initialisation
//======================================================

void setup()
{
    //==================================================
    // Port série
    //==================================================

    Serial.begin(9600);

    Serial.println();
    Serial.println(F("================================"));
    Serial.println(F("GDCC RECEPTEUR V3.1"));
    Serial.println(F("Initialisation..."));
    Serial.println(F("================================"));


    //==================================================
    // Initialisation modules
    //==================================================

    Motor_Init();

    Functions_Init();

    Battery_Init();

    Radio_Init();


    //==================================================
    // Etat initial sécurisé
    //==================================================

    Motor_Stop();

    Functions_LightOff();


    //==================================================
    // Initialisation du timeout radio
    //
    // Le récepteur démarre en sécurité.
    // Il attend le premier paquet valide.
    //==================================================

    lastRadioPacketTime = millis();

    radioConnected = false;


    Serial.println(F("Systeme pret"));
    Serial.println(F("Attente liaison radio..."));
}


//======================================================
// Boucle principale
//======================================================

void loop()
{
    //==================================================
    // Vérification réception radio
    //==================================================

    if (Radio_Available())
    {
        //==============================================
        // Lecture du paquet
        //==============================================

        if (Radio_Receive(radioPacket))
        {
            //==========================================
            // Paquet valide reçu
            //
            // Mise à jour du timer de sécurité
            //==========================================

            lastRadioPacketTime = millis();


            //==========================================
            // Réactivation de la liaison radio
            //==========================================

            if (!radioConnected)
            {
                radioConnected = true;

                Serial.println(
                    F("RADIO OK - LIAISON RETABLIE")
                );
            }


            //==========================================
            // ARU
            //==========================================

            if (radioPacket.ARU == 1)
            {
                Motor_Stop();

                Functions_LightOff();

                Serial.println(
                    F("ARU ACTIF")
                );
            }


            //==========================================
            // Batterie faible
            //==========================================

            else if (Battery_IsLow())
            {
                Motor_Stop();

                Functions_LightOff();

                Serial.println(
                    F("BATTERIE FAIBLE")
                );
            }


            //==========================================
            // Fonctionnement normal
            //==========================================

            else
            {
                //======================================
                // Commande moteur
                //======================================

                Motor_SetDirection(
                    radioPacket.direction
                );

                Motor_SetSpeed(
                    radioPacket.throttle
                );


                //======================================
                // Eclairage
                //======================================

                if (
                    radioPacket.LIGHT_Value == 1
                )
                {
                    Functions_LightOn();
                }
                else
                {
                    Functions_LightOff();
                }
            }
        }
    }


    //==================================================
    // TIMEOUT RADIO
    //
    // Vérification permanente de la liaison.
    //
    // Si aucun paquet valide n'a été reçu pendant
    // RADIO_TIMEOUT, le récepteur passe en sécurité.
    //==================================================

    if (
        radioConnected &&
        (
            millis() - lastRadioPacketTime >=
            RADIO_TIMEOUT
        )
    )
    {
        //==============================================
        // La liaison radio est considérée comme perdue
        //==============================================

        radioConnected = false;


        //==============================================
        // Mise en sécurité
        //==============================================

        Motor_Stop();

        Functions_LightOff();


        //==============================================
        // Message diagnostic
        //==============================================

        Serial.println(
            F("!!! RADIO LOST - SECURITE !!!")
        );
    }


    //==================================================
    // Surveillance batterie
    //
    // La batterie est contrôlée même sans réception
    // d'un nouveau paquet radio.
    //==================================================

    static unsigned long lastBatteryCheck = 0;

    if (
        millis() - lastBatteryCheck >=
        BATTERY_CHECK_INTERVAL
    )
    {
        lastBatteryCheck =
            millis();


        float batteryVoltage =
            Battery_ReadVoltage();


        Serial.print(
            F("Batterie : ")
        );

        Serial.print(
            batteryVoltage,
            2
        );

        Serial.println(
            F(" V")
        );


        //==============================================
        // Sécurité batterie faible
        //==============================================

        if (
            batteryVoltage <
            BATTERY_MIN_VOLTAGE
        )
        {
            Motor_Stop();

            Functions_LightOff();

            Serial.println(
                F("!!! BATTERIE FAIBLE !!!")
            );
        }
    }
}