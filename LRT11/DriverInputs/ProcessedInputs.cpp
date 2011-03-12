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

bool ProcessedInputs::ShouldBrakeLeft()
{
    return driverStick.IsButtonDown(10);
}

bool ProcessedInputs::ShouldBrakeRight()
{
    return driverStick.IsButtonDown(9);
}

bool ProcessedInputs::ShouldShiftLow()
{
    return driverStick.IsButtonJustPressed(3);
}

bool ProcessedInputs::ShouldShiftHigh()
{
    return driverStick.IsButtonJustPressed(4);
}

bool ProcessedInputs::ShouldMoveLiftLow()
{
    return operatorStick.IsButtonJustPressed(8);
}

bool ProcessedInputs::ShouldMoveLiftMedium()
{
    return operatorStick.IsButtonJustPressed(9);
}

bool ProcessedInputs::ShouldMoveLiftHigh()
{
    return operatorStick.IsButtonJustPressed(10);
}

bool ProcessedInputs::ShouldManuallyPowerLift()
{
    return Util::Abs<float>(Util::AddDeadband<float>(-operatorStick.GetY(), forwardDeadband)) > 0;
}

float ProcessedInputs::GetLiftPower()
{
    return 0.5 * -operatorStick.GetY();
}

bool ProcessedInputs::ShouldMoveArmDown()
{
    return operatorStick.IsButtonDown(7);
}

bool ProcessedInputs::ShouldMoveArmUp()
{
    return operatorStick.IsButtonDown(6);
}

bool ProcessedInputs::ShouldMoveArmBottomPreset()
{
    return driverStick.IsButtonJustPressed(8);
}

bool ProcessedInputs::ShouldMoveArmTopPreset()
{
    return driverStick.IsButtonJustPressed(7);
}

bool ProcessedInputs::ShouldRollerSpit()
{
    // TODO confirm button mapping
    return operatorStick.IsButtonDown(8);
}

bool ProcessedInputs::ShouldRollerSuck()
{
    // TODO confirm button mapping
    return operatorStick.IsButtonDown(9);
}

bool ProcessedInputs::ShouldRollerRotate()
{
    // TODO confirm button mapping
    return operatorStick.IsButtonDown(10);
}

bool ProcessedInputs::ShouldRollerBeAutomated()
{
    // TODO add correct button mapping
    return operatorStick.IsButtonDown(-1);
}

bool ProcessedInputs::ShouldRollerCommenceAutomation()
{
    // TODO add correct button mapping
    return operatorStick.IsButtonJustPressed(-1);
}

bool ProcessedInputs::GetOperatorThrottle()
{
    return -operatorStick.GetRawAxis(4) > 0.8;
}

bool ProcessedInputs::ShouldCollectEncoderData()
{
//    return driverStick.IsButtonJustPressed(1);
    return false;
}

void ProcessedInputs::UpdateDebouncing()
{
    driverStick.Update();
    operatorStick.Update();
}

bool ProcessedInputs::ShouldLoadConfig()
{
    return operatorStick.IsButtonJustPressed(15);
}

bool ProcessedInputs::ShouldSaveConfig()
{
    return operatorStick.IsButtonJustPressed(16);
}

bool ProcessedInputs::ShouldApplyConfig()
{
    return operatorStick.IsButtonJustPressed(14);
}
