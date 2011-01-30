#include "ProcessedInputs.h"

ProcessedInputs* ProcessedInputs::instance = NULL;

ProcessedInputs::ProcessedInputs()
    : driverStick(1, DriverStationConfig::NUM_JOYSTICK_BUTTONS,
            DriverStationConfig::NUM_JOYSTICK_AXES)
    , operatorStick(2, DriverStationConfig::NUM_JOYSTICK_BUTTONS,
            DriverStationConfig::NUM_JOYSTICK_AXES)
{
    AddToSingletonList();
    Config::RegisterConfigurable(this);
}

ProcessedInputs::~ProcessedInputs()
{
    // nothing to do
}

ProcessedInputs& ProcessedInputs::GetInstance()
{
    if(instance == NULL)
        instance = new ProcessedInputs();
    return *instance;
}

void ProcessedInputs::Configure()
{
    Config& config = Config::GetInstance();
    string prefix = "ProcessedJoystick.";

    forwardDeadband = config.Get<float>(prefix + "forwardDeadband", 0.15);
    turnDeadband = config.Get<float>(prefix + "turnDeadband", 0.25);
}

bool ProcessedInputs::IsShiftPressed()
{
    return driverStick.IsButtonDown(1);
}

float ProcessedInputs::GetForward()
{
    return Util::AddDeadband<float>(-driverStick.GetY(), forwardDeadband);
}

float ProcessedInputs::GetTurn()
{
    return Util::AddDeadband<float>(-driverStick.GetRawAxis(3), turnDeadband);
}

bool ProcessedInputs::GetBrakeLeft()
{
    return driverStick.IsButtonDown(11);
}

bool ProcessedInputs::GetBrakeRight()
{
    return driverStick.IsButtonDown(12);
}

bool ProcessedInputs::GetServiceMode()
{
    return GetThrottle() > 0.8;
}

float ProcessedInputs::GetThrottle()
{
    return -driverStick.GetRawAxis(4);
}

bool ProcessedInputs::ShouldLCDScrollUp()
{
    return driverStick.IsHatSwitchJustPressed(6, -1);
}

bool ProcessedInputs::ShouldLCDScrollDown()
{
    return driverStick.IsHatSwitchJustPressed(6, -1);
}

void ProcessedInputs::UpdateDebouncing()
{
    driverStick.Update();
    operatorStick.Update();
}
