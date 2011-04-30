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
//    Config& config = Config::GetInstance();
//    string prefix = "ProcessedInputs.";
}

float ProcessedInputs::GetThrottle()
{
    return -driverStick.GetRawAxis(4);
}

bool ProcessedInputs::IsServiceMode()
{
    return GetThrottle() > 0.5;
}

bool ProcessedInputs::IsDriverTriggerDown()
{
    return driverStick.IsButtonDown(1);
}

bool ProcessedInputs::IsOperatorTriggerDown()
{
    return operatorStick.IsButtonDown(1);
}

bool ProcessedInputs::ShouldAbort()
{
    return operatorStick.IsButtonDown(5) || driverStick.IsButtonDown(2);
}

float ProcessedInputs::GetForward()
{
    return -driverStick.GetY();
}

float ProcessedInputs::GetTurn()
{
    if(Util::Abs<float>(driverStick.GetRawAxis(3)) < 1e-6)
        return 0.0;

    return -driverStick.GetRawAxis(3);
}

bool ProcessedInputs::ShouldBrakeLeft()
{
    return driverStick.IsButtonDown(10);
}

bool ProcessedInputs::ShouldBrakeRight()
{
    return driverStick.IsButtonDown(9);
}

bool ProcessedInputs::ShouldShiftHigh()
{
    return driverStick.IsButtonDown(8);
}

bool ProcessedInputs::ShouldToggleGear()
{
    return driverStick.IsButtonJustPressed(8);
}

bool ProcessedInputs::ShouldShiftThird()
{
    return driverStick.IsButtonDown(9);
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
    return Util::Abs<float>(-operatorStick.GetY()) > 0.1;
}

float ProcessedInputs::GetLiftPower()
{
    // 50% max duty cycle to ensure that lift is controllable
    return -operatorStick.GetY();
}

bool ProcessedInputs::IsHighRow()
{
    return -operatorStick.GetRawAxis(4) > 0.5;
}

bool ProcessedInputs::ShouldMoveArmDown()
{
    return false;
    return operatorStick.IsButtonDown(12);
}

bool ProcessedInputs::ShouldMoveArmUp()
{
    return false;
    return operatorStick.IsButtonDown(11);
}

bool ProcessedInputs::ShouldMoveArmBottomPreset()
{
    return driverStick.IsButtonJustPressed(8);
}

bool ProcessedInputs::ShouldMoveArmTopPreset()
{
    return driverStick.IsButtonJustPressed(7);
}

bool ProcessedInputs::ShouldGrabGamePiece()
{
    return IsDriverTriggerDown();
}

bool ProcessedInputs::ShouldRollerSpit()
{
    return driverStick.IsButtonDown(3);
}

bool ProcessedInputs::ShouldRollerSuck()
{
    return IsOperatorTriggerDown() || driverStick.IsButtonDown(5);
}

bool ProcessedInputs::ShouldRollerRotateUp()
{
    return operatorStick.IsButtonDown(6);
}

bool ProcessedInputs::ShouldRollerRotateDown()
{
    return operatorStick.IsButtonDown(7);
}

bool ProcessedInputs::ShouldRollerBeAutomated()
{
    return operatorStick.IsButtonDown(2);
}

bool ProcessedInputs::ShouldRollerCommenceAutomation()
{
    return operatorStick.IsButtonJustPressed(12);
}

bool ProcessedInputs::GetOperatorThrottle()
{
    return -operatorStick.GetRawAxis(4) > 0.5;
}

bool ProcessedInputs::ShouldMoveArmToMiddle()
{
    return driverStick.IsButtonJustPressed(10);
}

bool ProcessedInputs::ShouldReleaseRingerWithArm()
{
    return operatorStick.IsButtonJustPressed(12);
}

bool ProcessedInputs::ShouldMoveArmUpAndLiftDown()
{
    return operatorStick.IsButtonJustPressed(11);
}

bool ProcessedInputs::ShouldReleaseRingerWithLift()
{
    return operatorStick.IsButtonJustPressed(2);
}

bool ProcessedInputs::ShouldDeployAligner()
{
    return operatorStick.IsButtonDown(3);
}

bool ProcessedInputs::ShouldDeployMinibot()
{
    return operatorStick.IsButtonDown(4);
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
    return IsOperatorTriggerDown() && operatorStick.IsButtonJustPressed(9);
}

bool ProcessedInputs::ShouldSaveConfig()
{
    return IsOperatorTriggerDown() && operatorStick.IsButtonJustPressed(8);
}

bool ProcessedInputs::ShouldApplyConfig()
{
    return IsOperatorTriggerDown() && operatorStick.IsButtonJustPressed(10);
}
