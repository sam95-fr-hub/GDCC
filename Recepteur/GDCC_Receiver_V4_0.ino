/******************************************************************************
*

* GDCC
* Récepteur locomotive V4.0
*
* Architecture radio V4.0 :
*
* * Toutes les locomotives écoutent la même adresse radio physique :
*
* ```
      GDCC_RADIO_NETWORK_ID
  ```
*
* * Chaque locomotive possède un identifiant logique unique :
*
* ```
      LOCO_ID
  ```
*
* * L'identifiant logique est transporté dans :
*
* ```
      RadioPacket.destination
  ```
*
* * Le récepteur traite uniquement les paquets dont :
*
* ```
      packet.destination == LOCO_ID
  ```
*
* Exemple :
*
* Réseau radio physique :
*
* ```
    GDCC_RADIO_NETWORK_ID = 200
  ```
*
* Loco 10 :
*
* ```
    LOCO_ID = 10
  ```
*
* Loco 11 :
*
*
    LOCO_ID = 11

*
* Loco 12 :
*
*
    LOCO_ID = 12
*
* La télécommande envoie une trame vers l'adresse radio
* physique commune.
*
* Chaque locomotive reçoit physiquement la trame et vérifie
* ensuite si elle lui est destinée.
*
* Seule la locomotive correspondant à :
*
* ```
  packet.destination
  ```
*
* exécute la commande.
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
//
// Cette structure contient le dernier paquet valide
// reçu et destiné à cette locomotive.
//
// Radio_Receive() effectue notamment le filtrage sur :
//
//     packet.destination == LOCO_ID
//
//======================================================

RadioPacket radioPacket;

//======================================================
// Gestion du timeout radio
//======================================================
//
// Moment de réception du dernier paquet radio valide
// et destiné à cette locomotive.
//
// IMPORTANT :
//
// Un paquet destiné à une autre locomotive ne réarme
// PAS le timeout.
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
// La liaison devient active uniquement après réception
// d'un paquet valide destiné à cette locomotive.
//
//======================================================

bool radioConnected = false;

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
    F("GDCC RECEPTEUR V4.0")
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
//
// Activée uniquement si :
//
//     BATTERY_MONITOR_ENABLED == true
//
// Si false :
//
//   - Battery_Init() n'est pas appelé
//   - aucune mesure batterie n'est effectuée
//
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
//
// V4.0 :
//
// Le NRF24L01 du récepteur est initialisé avec :
//
//     GDCC_RADIO_NETWORK_ID
//
// Cette adresse est commune à toutes les locomotives.
//
// L'identité individuelle de la locomotive n'est PAS
// utilisée comme adresse physique du NRF24L01.
//
// L'identité logique de la locomotive est :
//
//     LOCO_ID
//
// Elle est comparée à :
//
//     packet.destination
//
//==================================================

Radio_Init();


//==================================================
// Etat initial sécurisé
//==================================================
//
// La locomotive démarre toujours :
//
//   - moteur arrêté
//   - éclairage éteint
//
//==================================================

Motor_Stop();

Functions_LightOff();


//==================================================
// Initialisation du timeout radio
//==================================================
//
// Le récepteur démarre en sécurité.
//
// Il attend la réception d'un premier paquet valide
// destiné à cette locomotive.
//
//==================================================

lastRadioPacketTime =
    millis();

radioConnected =
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
//
// Radio_Available() vérifie si un paquet est disponible
// dans le NRF24L01.
//
// Si un paquet est disponible, Radio_Receive() :
//
//   1. lit le paquet
//   2. vérifie sa destination
//   3. vérifie son type de commande
//
// Un paquet destiné à une autre locomotive est ignoré.
//
//==================================================


if (
    Radio_Available()
)
{
    //==============================================
    // Lecture et filtrage du paquet
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
        // Le paquet est destiné à cette locomotive.
        //
        // Le timer de sécurité est donc réarmé.
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
        // Si ARU == 1 :
        //
        //   - arrêt immédiat du moteur
        //   - extinction de l'éclairage
        //   - aucune commande normale n'est exécutée
        //
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
        //
        // La commande n'est exécutée que si :
        //
        //   - le paquet est destiné à cette loco
        //   - la commande est CMD_DRIVE
        //   - l'ARU est inactif
        //   - la batterie n'est pas trop faible
        //
        //==========================================

        else
        {
            Serial.println(
                F(">>> COMMANDE NORMALE <<<")
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
// 2. TIMEOUT RADIO
//==================================================
//
// Vérification permanente de la liaison.
//
// Si aucun paquet valide destiné à cette locomotive
// n'a été reçu pendant RADIO_TIMEOUT :
//
//   - la liaison est considérée comme perdue
//   - le moteur est arrêté
//   - l'éclairage est éteint
//
// Cette sécurité protège également la locomotive
// en cas de :
//
//   - extinction de la télécommande
//   - perte de portée radio
//   - panne du NRF24L01
//   - interruption du programme de la télécommande
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
//
// La batterie est contrôlée indépendamment
// de la réception radio.
//
// Cette partie est compilée uniquement si :
//
//     BATTERY_MONITOR_ENABLED == true
//
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
