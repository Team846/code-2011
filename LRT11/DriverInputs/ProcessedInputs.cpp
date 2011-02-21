#include "ProcessedInputs.h"

ProcessedInputs* ProcessedInputs::instance = NULL;

ProcessedInputs::ProcessedInputs()
    : driverStick(1, DriverStationConfig::NUM_JOYSTICK_BUTTONS,
            DriverStationConfig::NUM_JOYSTICK_AXES)
    , operatorStick(2, DriverStationConfig::NUM_JOYSTICK_BUTTONS,
            DriverStationConfig::NUM_JOYSTICK_AXES)
{
    AddToSingletonList();
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

float ProcessedInputs::GetThrottle()
{
    return -driverStick.GetRawAxis(4);
}

bool ProcessedInputs::IsServiceMode()
{
    return GetThrottle() > 0.8;
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

float ProcessedInputs::GetOperatorForwardScaled30()
{
    // in range [0, 0.3]
    return .55 * Util::AddDeadband<float>(-operatorStick.GetY(), forwardDeadband);
}

bool ProcessedInputs::GetBrakeLeft()
{
    return driverStick.IsButtonDown(11);
}

bool ProcessedInputs::GetBrakeRight()
{
    return driverStick.IsButtonDown(12);
}

bool ProcessedInputs::ShouldCollectEncoderData()
{
    return driverStick.IsButtonJustPressed(1);
}

void ProcessedInputs::UpdateDebouncing()
{
    driverStick.Update();
    operatorStick.Update();
}

bool ProcessedInputs::ShouldLoadConfig()
{
    return operatorStick.IsButtonJustPressed(8);
}

bool ProcessedInputs::ShouldSaveConfig()
{
    return operatorStick.IsButtonJustPressed(9);
}

bool ProcessedInputs::ShouldApplyConfig()
{
    return operatorStick.IsButtonJustPressed(10);
}
