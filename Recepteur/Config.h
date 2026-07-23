/******************************************************************************
 *
 * GDCC
 * Configuration générale du récepteur locomotive
 *
 * Version V3.2
 * Architecture multi-locomotives
 *
 * Ce fichier sélectionne automatiquement la configuration
 * de la locomotive à compiler.
 *
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


//======================================================
// SELECTION DE LA LOCOMOTIVE
//======================================================
//
// Modifier UNIQUEMENT cette valeur pour compiler
// le programme destiné à une autre locomotive.
//
// 10 = Config_Loco10.h
// 11 = Config_Loco11.h
// 12 = Config_Loco12.h
//
// Exemple :
//
// #define LOCO_CONFIG 10
//
// compile la configuration de la loco 10.
//
//======================================================

#define LOCO_CONFIG 12


//======================================================
// CHARGEMENT DE LA CONFIGURATION
//======================================================
//
// Le bon fichier Config_LocoXX.h est chargé
// automatiquement selon LOCO_CONFIG.
//
//======================================================

#if LOCO_CONFIG == 10

    #include "Config_Loco10.h"

#elif LOCO_CONFIG == 11

    #include "Config_Loco11.h"

#elif LOCO_CONFIG == 12

    #include "Config_Loco12.h"

#else

    #error "ERREUR : LOCO_CONFIG invalide. Utiliser 10, 11 ou 12."

#endif


#endif
