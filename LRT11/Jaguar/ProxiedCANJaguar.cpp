#include "ProxiedCANJaguar.h"

ProxiedCANJaguar::ProxiedCANJaguar(UINT8 channel) :
	controller(CANBusController::GetInstance())
    , channel(channel)
{

}

void ProxiedCANJaguar::Set(float speed, UINT8 syncGroup)
{
    controller.Set(channel, Util::Clamp<float>(speed, -1.0, 1.0));
}

float ProxiedCANJaguar::Get()
{
    return controller.Get(channel);
}

void ProxiedCANJaguar::Disable()
{
	controller.Set(channel, 0);
}

float ProxiedCANJaguar::GetCurrent()
{
    return controller.GetCurrent(channel);
}

float ProxiedCANJaguar::GetTemperature()
{
    return controller.GetTemperature(channel);
}

float ProxiedCANJaguar::GetBatteryVoltage()
{
    return controller.GetBatteryVoltage(channel);
}

float ProxiedCANJaguar::GetOutputVoltage()
{
    return controller.GetOutputVoltage(channel);
}

void ProxiedCANJaguar::ConfigNeutralMode(CANJaguar::NeutralMode mode)
{
    controller.ConfigNeutralMode(channel, mode);
}

