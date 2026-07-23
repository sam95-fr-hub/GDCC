/******************************************************************************
 *
 * GDCC
 * Gestion du moteur locomotive
 *
 * Version V3.2
 * Architecture modulaire
 *
 * Drivers supportés :
 *
 *   - L298N
 *   - DRV8871
 *
 * Le type de driver est sélectionné dans Config.h.
 *
 ******************************************************************************/

#include "Motor.h"
#include "Config.h"


//======================================================
// Direction actuelle du moteur
//
// Utilisée principalement pour le DRV8871.
//
// 0 = arrière
// 1 = avant
//======================================================

static uint8_t currentDirection = 1;


//======================================================
// Vitesse actuelle du moteur
//
// Utilisée pour permettre au DRV8871 de recevoir
// la bonne valeur PWM lorsque la direction change.
//
// 0   = arrêt
// 255 = vitesse maximale
//======================================================

static uint8_t currentSpeed = 0;


//======================================================
// Initialisation du moteur
//======================================================

void Motor_Init()
{
    //==================================================
    // Configuration des broches moteur
    //==================================================

    pinMode(
        PIN_MOTOR_IN1,
        OUTPUT
    );

    pinMode(
        PIN_MOTOR_IN2,
        OUTPUT
    );


    //==================================================
    // Configuration spécifique au L298N
    //
    // ENA est utilisé pour le PWM de vitesse.
    //==================================================

#if defined(MOTOR_DRIVER_L298N)

    pinMode(
        PIN_MOTOR_ENA,
        OUTPUT
    );

#endif


    //==================================================
    // Etat initial sécurisé
    //==================================================

    currentDirection = 1;

    currentSpeed = 0;

    Motor_Stop();
}


//======================================================
// Réglage de la direction
//======================================================
//
// direction :
//
//   0 = arrière
//   1 = avant
//
// MOTOR_REVERSED permet d'inverser le sens physique
// du moteur sans modifier le code.
//
//======================================================

void Motor_SetDirection(
    uint8_t direction
)
{
    //==================================================
    // Mémorisation de la direction demandée
    //==================================================

    currentDirection =
        (
            direction == 1
        )
        ? 1
        : 0;


    //==================================================
    // Détermination du sens physique
    //==================================================

    bool forward =
        (
            currentDirection == 1
        );


    //==================================================
    // Inversion du sens physique si nécessaire
    //==================================================

#if MOTOR_REVERSED

    forward =
        !forward;

#endif


    //==================================================
    // DRIVER L298N
    //==================================================
//
// Le L298N utilise :
//
//   IN1 / IN2 = direction
//   ENA       = PWM vitesse
//
// La vitesse est donc gérée séparément dans
// Motor_SetSpeed().
//

#if defined(MOTOR_DRIVER_L298N)

    if (forward)
    {
        //==============================================
        // Marche avant
        //==============================================

        digitalWrite(
            PIN_MOTOR_IN1,
            LOW
        );

        digitalWrite(
            PIN_MOTOR_IN2,
            HIGH
        );
    }
    else
    {
        //==============================================
        // Marche arrière
        //==============================================

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
    // DRIVER DRV8871
    //==================================================
//
// Le DRV8871 utilise ses deux entrées pour la commande
// du sens et de la vitesse.
//
//   AVANT   : IN1 = LOW / IN2 = PWM
//   ARRIERE : IN1 = PWM / IN2 = LOW
//
// La valeur PWM actuelle est réappliquée afin de
// permettre un changement de direction en conservant
// la vitesse actuelle.
//

#elif defined(MOTOR_DRIVER_DRV8871)

    if (forward)
    {
        //==============================================
        // Marche avant
        //==============================================

        analogWrite(
            PIN_MOTOR_IN1,
            0
        );

        analogWrite(
            PIN_MOTOR_IN2,
            currentSpeed
        );
    }
    else
    {
        //==============================================
        // Marche arrière
        //==============================================

        analogWrite(
            PIN_MOTOR_IN1,
            currentSpeed
        );

        analogWrite(
            PIN_MOTOR_IN2,
            0
        );
    }

#endif
}


//======================================================
// Réglage de la vitesse
//======================================================
//
// throttle :
//
//   0   = arrêt
//   255 = vitesse maximale
//
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
    // Mémorisation de la vitesse
    //==================================================

    currentSpeed =
        throttle;


    //==================================================
    // DRIVER L298N
    //==================================================
//
// La vitesse est appliquée sur ENA.
//

#if defined(MOTOR_DRIVER_L298N)

    analogWrite(
        PIN_MOTOR_ENA,
        currentSpeed
    );


    //==================================================
    // DRIVER DRV8871
    //==================================================
//
// La vitesse est appliquée sur l'entrée correspondant
// à la direction actuelle.
//
//   AVANT   : IN1 = LOW / IN2 = PWM
//   ARRIERE : IN1 = PWM / IN2 = LOW
//

#elif defined(MOTOR_DRIVER_DRV8871)

    //==================================================
    // Détermination du sens physique
    //==================================================

    bool forward =
        (
            currentDirection == 1
        );


    //==================================================
    // Inversion du sens physique si nécessaire
    //==================================================

#if MOTOR_REVERSED

    forward =
        !forward;

#endif


    if (forward)
    {
        //==============================================
        // Marche avant
        //==============================================

        analogWrite(
            PIN_MOTOR_IN1,
            0
        );

        analogWrite(
            PIN_MOTOR_IN2,
            currentSpeed
        );
    }
    else
    {
        //==============================================
        // Marche arrière
        //==============================================

        analogWrite(
            PIN_MOTOR_IN1,
            currentSpeed
        );

        analogWrite(
            PIN_MOTOR_IN2,
            0
        );
    }

#endif
}


//======================================================
// Arrêt immédiat du moteur
//======================================================

void Motor_Stop()
{
    //==================================================
    // Réinitialisation de la vitesse
    //==================================================

    currentSpeed =
        0;


    //==================================================
    // DRIVER L298N
    //==================================================

#if defined(MOTOR_DRIVER_L298N)

    //==============================================
    // Arrêt PWM
    //==============================================

    analogWrite(
        PIN_MOTOR_ENA,
        0
    );


    //==============================================
    // Mise à LOW des entrées
    //==============================================

    digitalWrite(
        PIN_MOTOR_IN1,
        LOW
    );

    digitalWrite(
        PIN_MOTOR_IN2,
        LOW
    );


    //==================================================
    // DRIVER DRV8871
    //==================================================

#elif defined(MOTOR_DRIVER_DRV8871)

    //==============================================
    // Arrêt moteur
    //==============================================

    analogWrite(
        PIN_MOTOR_IN1,
        0
    );

    analogWrite(
        PIN_MOTOR_IN2,
        0
    );

#endif
}
