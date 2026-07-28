/******************************************************************************
*

* GDCC
* Récepteur locomotive V4.1
*
* Architecture radio V4.1 :
*
* Toutes les locomotives écoutent la même adresse radio physique :
*
* ```
  GDCC_RADIO_NETWORK_ID
  ```
*
* Chaque locomotive possède un identifiant logique unique :
*
* ```
  LOCO_ID
  ```
*
* L'identifiant logique est transporté dans :
*
* ```
  RadioPacket.destination
  ```
*
* Une commande normale CMD_DRIVE est traitée uniquement si :
*
* ```
  packet.destination == LOCO_ID
  ```
*
* Une commande CMD_EMERGENCY_STOP avec :
*
* ```
  packet.destination == GDCC_DEST_BROADCAST
  ```
*
* est traitée par toutes les locomotives.
*
* Une commande CMD_EMERGENCY_RELEASE avec :
*
* ```
  packet.destination == GDCC_DEST_BROADCAST
  ```
*
* fait sortir toutes les locomotives de l'état ARU.
*
* L'ARU broadcast est prioritaire sur toute commande normale.
*
* Après réception d'un ARU :
*
* * le moteur est arrêté immédiatement
* * l'éclairage est éteint
* * la locomotive reste bloquée en sécurité
* * les commandes CMD_DRIVE sont ignorées
*
* Après réception de CMD_EMERGENCY_RELEASE :
*
* * l'état ARU est désactivé
* * le moteur reste arrêté
* * l'éclairage reste éteint
* * aucun redémarrage automatique n'est effectué
*
* Une nouvelle commande CMD_DRIVE destinée à cette locomotive
* est nécessaire pour reprendre le fonctionnement manuel.
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
//
// Moment de réception du dernier paquet radio valide
// destiné à cette locomotive.
//
// Un paquet ARU broadcast réarme également le timer,
// car il constitue une communication radio valide.
//
// Un paquet CMD_EMERGENCY_RELEASE broadcast réarme
// également le timer.
//
//======================================================

unsigned long lastRadioPacketTime = 0;

//======================================================
// Etat actuel de la liaison radio
//======================================================
//
// false = liaison perdue
// true  = liaison active
//
// La liaison devient active après réception d'un paquet
// valide destiné à cette locomotive ou d'une commande
// ARU broadcast.
//
//======================================================

bool radioConnected = false;

//======================================================
// Etat local de l'ARU
//======================================================
//
// true  = ARU actif pour cette locomotive
// false = fonctionnement normal autorisé
//
// Cet état est mémorisé localement.
//
// Après réception de CMD_EMERGENCY_STOP broadcast :
//
//   emergencyStopActive = true
//
// Tant que cet état est actif :
//
//   - le moteur reste arrêté
//   - l'éclairage reste éteint
//   - CMD_DRIVE est ignoré
//
// L'état ARU est désactivé uniquement par la réception
// de CMD_EMERGENCY_RELEASE broadcast.
//
// Après la sortie de l'ARU, la locomotive reste arrêtée
// et attend un nouveau CMD_DRIVE.
//
//======================================================

bool emergencyStopActive = false;

//======================================================
// SETUP
//======================================================

void setup()
{
//==================================================
// Port série
//==================================================


Serial.begin(9600);

delay(500);


Serial.println();

Serial.println(
    F("================================")
);

Serial.println(
    F("GDCC RECEPTEUR V4.1")
);

Serial.println(
    F("Initialisation...")
);

Serial.println(
    F("================================")
);


//==================================================
// Initialisation du moteur
//==================================================

Motor_Init();

Serial.println(
    F("MOTEUR OK")
);


//==================================================
// Initialisation des fonctions
//==================================================

Functions_Init();

Serial.println(
    F("FONCTIONS OK")
);


//==================================================
// Initialisation surveillance batterie
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
//==================================================

lastRadioPacketTime =
    millis();

radioConnected =
    false;


//==================================================
// Initialisation de l'état ARU
//==================================================

emergencyStopActive =
    false;


//==================================================
// Diagnostic
//==================================================

Serial.println();

Serial.print(
    F("LOCO ID : ")
);

Serial.println(
    LOCO_ID
);


Serial.print(
    F("RESEAU RADIO GDCC : ")
);

Serial.println(
    GDCC_RADIO_NETWORK_ID
);


Serial.print(
    F("DESTINATION BROADCAST : ")
);

Serial.println(
    GDCC_DEST_BROADCAST
);


Serial.println(
    F("SYSTEME PRET")
);

Serial.println(
    F("Attente liaison radio...")
);


}

//======================================================
// LOOP
//======================================================

void loop()
{
//==================================================
// 1. Vérification de la réception radio
//==================================================


if (
    Radio_Available()
)
{
    //==============================================
    // Lecture et filtrage du paquet
    //
    // Radio_Receive() accepte :
    //
    //   - CMD_DRIVE destiné à LOCO_ID
    //   - CMD_EMERGENCY_STOP destiné au broadcast
    //   - CMD_EMERGENCY_RELEASE destiné au broadcast
    //
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
        // Le paquet est soit :
        //
        //   - destiné à cette locomotive
        //   - un ARU broadcast
        //   - une sortie d'ARU broadcast
        //
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
        // ARRET D'URGENCE BROADCAST
        //==========================================
        //
        // La commande CMD_EMERGENCY_STOP est
        // prioritaire sur toute commande normale.
        //
        // Le broadcast est destiné à toutes les
        // locomotives du réseau.
        //
        //==========================================

        if (
            radioPacket.command ==
            CMD_EMERGENCY_STOP
        )
        {
            //======================================
            // Activation de l'état ARU local
            //======================================

            emergencyStopActive =
                true;


            Serial.println(
                F(">>> ARU BROADCAST : ARRET FORCE <<<")
            );


            //======================================
            // Arrêt immédiat du moteur
            //======================================

            Motor_Stop();


            //======================================
            // Extinction de l'éclairage
            //======================================

            Functions_LightOff();
        }


        //==========================================
        // SORTIE DE L'ARU BROADCAST
        //==========================================
        //
        // CMD_EMERGENCY_RELEASE signifie uniquement
        // que le mode ARU global est terminé.
        //
        // Cette commande ne provoque aucun
        // redémarrage automatique.
        //
        // Le moteur reste arrêté et l'éclairage
        // reste éteint.
        //
        // Un nouveau CMD_DRIVE sera nécessaire
        // pour reprendre la marche.
        //
        //==========================================

        else if (
            radioPacket.command ==
            CMD_EMERGENCY_RELEASE
        )
        {
            //======================================
            // Sortie de l'état ARU
            //======================================

            emergencyStopActive =
                false;


            Serial.println(
                F("<<< FIN ARU GLOBAL : LOCOMOTIVE DEBLOQUEE <<<")
            );


            //======================================
            // Maintien de l'arrêt physique
            //
            // Aucun redémarrage automatique.
            //======================================

            Motor_Stop();


            //======================================
            // Maintien de l'éclairage éteint
            //
            // Un nouveau CMD_DRIVE devra définir
            // explicitement l'état de l'éclairage.
            //======================================

            Functions_LightOff();
        }


        //==========================================
        // COMMANDE NORMALE
        //==========================================
        //
        // Une commande CMD_DRIVE valide est
        // destinée à cette locomotive.
        //
        // IMPORTANT :
        //
        // Si l'ARU est encore actif, la commande
        // CMD_DRIVE est totalement ignorée.
        //
        // Seul CMD_EMERGENCY_RELEASE broadcast
        // peut désactiver l'état ARU.
        //
        // Après la sortie d'ARU, un nouveau
        // CMD_DRIVE peut alors être exécuté.
        //
        //==========================================

        else if (
            radioPacket.command ==
            CMD_DRIVE
        )
        {
            //======================================
            // Priorité absolue de l'ARU
            //
            // Tant que l'ARU est actif, aucune
            // commande normale ne doit être exécutée.
            //======================================

            if (
                emergencyStopActive
            )
            {
                Serial.println(
                    F("CMD_DRIVE IGNOREE - ARU ACTIF")
                );
            }
            else
            {
                //==================================
                // Sécurité batterie
                //==================================


#if BATTERY_MONITOR_ENABLED


                if (
                    Battery_IsLow()
                )
                {
                    Serial.println(
                        F(">>> BATTERIE FAIBLE : ARRET FORCE <<<")
                    );


                    Motor_Stop();

                    Functions_LightOff();
                }
                else
                {
                    //==================================
                    // Commande moteur
                    //==================================

                    Motor_SetDirection(
                        radioPacket.direction
                    );

                    Motor_SetSpeed(
                        radioPacket.throttle
                    );


                    //==================================
                    // Eclairage
                    //==================================

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


#else


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


#endif
}
}
}
}


//==================================================
// 2. TIMEOUT RADIO
//==================================================
//
// Vérification permanente de la liaison.
//
// Si aucun paquet valide destiné à cette locomotive
// ou aucun paquet broadcast ARU / sortie ARU n'a été
// reçu pendant RADIO_TIMEOUT :
//
//   - liaison considérée comme perdue
//   - moteur arrêté
//   - éclairage éteint
//
// L'état ARU n'est pas modifié par le timeout.
//
// Ainsi, si la liaison est perdue pendant un ARU,
// la locomotive reste bloquée en sécurité jusqu'à
// réception de CMD_EMERGENCY_RELEASE.
//
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
// 3. SURVEILLANCE BATTERIE
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


    //==============================================
    // Lecture tension batterie
    //==============================================

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
