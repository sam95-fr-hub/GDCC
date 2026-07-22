/******************************************************************************
 *
 * GDCC
 * Gestion du moteur locomotive
 *
 * Version V3.1
 * Architecture modulaire
 *
 ******************************************************************************/

#include "Motor.h"
#include "Config.h"


//======================================================
// Initialisation du moteur
//======================================================

void Motor_Init()
{
    //==================================================
    // Configuration des broches L298N
    //==================================================

    pinMode(
        PIN_MOTOR_IN1,
        OUTPUT
    );

    pinMode(
        PIN_MOTOR_IN2,
        OUTPUT
    );

    pinMode(
        PIN_MOTOR_ENA,
        OUTPUT
    );


    //==================================================
    // Etat initial sécurisé
    //==================================================

    Motor_Stop();
}


//======================================================
// Réglage de la direction
//======================================================

void Motor_SetDirection(
    uint8_t direction
)
{
    //==================================================
    // Marche avant
    //==================================================

    if (
        direction == 1
    )
    {
        digitalWrite(
            PIN_MOTOR_IN1,
            HIGH
        );

        digitalWrite(
            PIN_MOTOR_IN2,
            LOW
        );
    }


    //==================================================
    // Marche arrière
    //==================================================

    else
    {
        digitalWrite(
            PIN_MOTOR_IN1,
            LOW
        );

        digitalWrite(
            PIN_MOTOR_IN2,
            HIGH
        );
    }
}


//======================================================
// Réglage de la vitesse
//======================================================

void Motor_SetSpeed(
    uint8_t throttle
)
{
    //==================================================
    // Sécurité : vitesse maximale 255
    //==================================================

    if (
        throttle > 255
    )
    {
        throttle = 255;
    }


    //==================================================
    // Commande PWM du moteur
    //==================================================

    analogWrite(
        PIN_MOTOR_ENA,
        throttle
    );
}


//======================================================
// Arrêt immédiat du moteur
//======================================================

void Motor_Stop()
{
    //==================================================
    // Arrêt PWM
    //==================================================

    analogWrite(
        PIN_MOTOR_ENA,
        0
    );


    //==================================================
    // Freinage par mise à LOW des entrées
    //==================================================

    digitalWrite(
        PIN_MOTOR_IN1,
        LOW
    );

    digitalWrite(
        PIN_MOTOR_IN2,
        LOW
    );
}