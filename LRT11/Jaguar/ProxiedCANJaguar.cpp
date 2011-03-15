#include "ProxiedCANJaguar.h"

ProxiedCANJaguar::ProxiedCANJaguar(UINT8 channel)
    : CANJaguar(channel)
#ifdef VIRTUAL
    , controller(VirtualCANBusController::GetInstance())
    , channel(channel)
#else
//    : controller(CANBusController::GetInstance())
#endif
{

}

ProxiedCANJaguar::~ProxiedCANJaguar()
{

}

#ifdef VIRTUAL
void ProxiedCANJaguar::Set(float setpoint, UINT8 syncGroup)
{
    controller.Set(channel, setpoint);
}

void ProxiedCANJaguar::SetPID(double p, double i, double d)
{
    controller.SetPID(channel, p, i, d);
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
    return controller.GetOutputCurrent(channel);
}

float ProxiedCANJaguar::GetTemperature()
{
    return controller.GetTemperature(channel);
}

float ProxiedCANJaguar::GetBatteryVoltage()
{
    return controller.GetBusVoltage(channel);
}

float ProxiedCANJaguar::GetOutputVoltage()
{
    return controller.GetOutputVoltage(channel);
}

double ProxiedCANJaguar::GetPosition()
{
    return controller.GetPosition(channel);
}

double ProxiedCANJaguar::GetSpeed()
{
    return controller.GetSpeed(channel);
}

void ProxiedCANJaguar::ConfigNeutralMode(CANJaguar::NeutralMode mode)
{
    controller.ConfigNeutralMode(channel, mode);
}

void ProxiedCANJaguar::SetPositionReference(VirtualPot* reference)
{
    controller.SetPositionReference(channel, reference);
}

void ProxiedCANJaguar::ConfigPotentiometerTurns(UINT16 turns)
{
    controller.SetPotentiometerTurns(channel, turns);
}

CANJaguar::PositionReference ProxiedCANJaguar::GetPositionReference()
{
    return controller.GetPositionReference(channel);
}

void ProxiedCANJaguar::ChangeControlMode(CANJaguar::ControlMode controlMode)
{
    controller.SetControlMode(channel, controlMode);
}

CANJaguar::ControlMode ProxiedCANJaguar::GetControlMode()
{
    return controller.GetControlMode(channel);
}

void ProxiedCANJaguar::EnableControl(double encoderInitialPosition)
{
    controller.EnableControl(channel, encoderInitialPosition);
}

void ProxiedCANJaguar::DisableControl()
{
    controller.DisableControl(channel);
}

void ProxiedCANJaguar::ResetCache()
{
    controller.ResetCache(channel);
}
#endif
