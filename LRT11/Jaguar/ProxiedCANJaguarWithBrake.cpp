#include "ProxiedCANJaguarWithBrake.h"

ProxiedCANJaguarWithBrake::ProxiedCANJaguarWithBrake(UINT8 channel)
    : ProxiedCANJaguar(channel)
    , brake(*this)
{

}

void ProxiedCANJaguarWithBrake::ApplyBrakes(int brakeAmount)
{
    brake.ApplyBrakes(brakeAmount);
}

void ProxiedCANJaguarWithBrake::UpdateOutput()
{
    brake.UpdateOutput();
}
