/******************************************************************************
 *
 * GDCC
 * Garden Digital Command Control
 *
 * Version 3.0.0-alpha1
 *
 ******************************************************************************/

#include "Version.h"
#include "Config.h"
#include "Types.h"

#include "Inputs.h"
#include "Battery.h"

HandsetState handset;

void setup()
{
    Inputs_Init();
    Battery_Init();
}

void loop()
{
    Inputs_Update(handset);

    handset.batteryVoltage = Battery_ReadVoltage();
}