/******************************************************************************
 *
 * GDCC
 * Récepteur locomotive
 *
 * Version V3.1 / V3.2
 * Architecture modulaire
 *
 * Matériel :
 *   Arduino Nano
 *   NRF24L01
 *   L298N / DRV8871
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

    Serial.println(
        F("================================")
    );

    Serial.println(
        F("GDCC RECEPTEUR V3.2")
    );

    Serial.println(
        F("Initialisation...")
    );

    Serial.println(
        F("================================")
    );


    //==================================================
    // Initialisation modules
    //==================================================

    Motor_Init();

    Functions_Init();


    //==================================================
    // Initialisation surveillance batterie
    //
    // Activée uniquement si :
    //
    // BATTERY_MONITOR_ENABLED == true
    //
    // Si false :
    // - Battery_Init() n'est pas appelé
    // - aucune mesure batterie n'est effectuée
    //==================================================

#if BATTERY_MONITOR_ENABLED

    Battery_Init();

    Serial.println(
        F("Surveillance batterie : ACTIVE")
    );

#else

    Serial.println(
        F("Surveillance batterie : DESACTIVEE")
    );

#endif


    //==================================================
    // Initialisation radio
    //==================================================

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

    lastRadioPacketTime =
        millis();

    radioConnected =
        false;


    Serial.println(
        F("Systeme pret")
    );

    Serial.println(
        F("Attente liaison radio...")
    );
}


//======================================================
// Boucle principale
//======================================================

void loop()
{
    //==================================================
    // Vérification réception radio
    //==================================================

    if (
        Radio_Available()
    )
    {
        //==============================================
        // Lecture du paquet
        //==============================================

        if (
            Radio_Receive(
                radioPacket
            )
        )
        {
            //==========================================
            // Paquet valide reçu
            //
            // Mise à jour du timer de sécurité
            //==========================================

            lastRadioPacketTime =
                millis();


            //==========================================
            // Réactivation de la liaison radio
            //==========================================

            if (
                !radioConnected
            )
            {
                radioConnected =
                    true;

                Serial.println(
                    F("RADIO OK - LIAISON RETABLIE")
                );
            }


            //==========================================
            // ARRET D'URGENCE
            //==========================================
            //
            // IMPORTANT :
            //
            // Si ARU == 1, on arrête le moteur
            // et on n'exécute PAS la commande normale.
            //
            // Le "else" garantit que les commandes
            // moteur normales sont mutuellement
            // exclusives avec l'ARU.
            //==========================================

            if (
                radioPacket.ARU == 1
            )
            {
                Serial.println(
                    F(">>> ARU ACTIF : ARRET FORCE <<<")
                );


                //======================================
                // Arrêt moteur
                //======================================

                Motor_Stop();


                //======================================
                // Extinction éclairage
                //======================================

                Functions_LightOff();
            }


            //==========================================
            // BATTERIE FAIBLE
            //==========================================

#if BATTERY_MONITOR_ENABLED

            else if (
                Battery_IsLow()
            )
            {
                Serial.println(
                    F(">>> BATTERIE FAIBLE : ARRET FORCE <<<")
                );


                //======================================
                // Arrêt moteur
                //======================================

                Motor_Stop();


                //======================================
                // Extinction éclairage
                //======================================

                Functions_LightOff();
            }

#endif


            //==========================================
            // FONCTIONNEMENT NORMAL
            //==========================================

#if BATTERY_MONITOR_ENABLED

            else

#else

            else

#endif

            {
                Serial.println(
                    F(">>> ARU INACTIF : COMMANDE NORMALE <<<")
                );


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
            millis() -
            lastRadioPacketTime >=
            RADIO_TIMEOUT
        )
    )
    {
        //==============================================
        // La liaison radio est considérée comme perdue
        //==============================================

        radioConnected =
            false;


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
    //
    // Cette partie est compilée uniquement si :
    //
    // BATTERY_MONITOR_ENABLED == true
    //
    // Si false :
    // aucune lecture de A2 n'est effectuée.
    //==================================================

#if BATTERY_MONITOR_ENABLED

    static unsigned long lastBatteryCheck =
        0;


    if (
        millis() -
        lastBatteryCheck >=
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

#endif
}
