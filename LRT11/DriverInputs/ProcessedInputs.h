#ifndef PROCESSED_JOYSTICK_H_
#define PROCESSED_JOYSTICK_H_

#include "..\General.h"
#include "DebouncedJoystick.h"
#include "..\Config\DriverStationConfig.h"
#include "..\Config\Config.h"
#include "..\Config\Configurable.h"
#include "..\Util\Util.h"

class ProcessedInputs : public SensorBase, public Configurable
{
private:
    static ProcessedInputs* instance;

    double forwardDeadband;
    double turnDeadband;

    float GetThrottle();

    DebouncedJoystick driverStick, operatorStick;

protected:
    ProcessedInputs();
    DISALLOW_COPY_AND_ASSIGN(ProcessedInputs);

public:
    ~ProcessedInputs();

    static ProcessedInputs& GetInstance();
    virtual void Configure();

    bool IsServiceMode();
    bool IsDriverTriggerDown();
    bool IsOperatorTriggerDown();

    // drive train
    float GetForward();
    float GetTurn();

    bool ShouldBrakeLeft();
    bool ShouldBrakeRight();

    // lift
    bool ShouldMoveLiftLow();
    bool ShouldMoveLiftMedium();
    bool ShouldMoveLiftHigh();
    bool ShouldManuallyPowerLift();
    float GetLiftPower();

    // arm
    bool ShouldMoveArmDown();
    bool ShouldMoveArmUp();
    bool ShouldMoveArmBottomPreset();
    bool ShouldMoveArmTopPreset();

    // roller
    bool ShouldRollerSpit();
    bool ShouldRollerSuck();
    bool ShouldRollerRotate();
    bool ShouldRollerBeAutomated();
    bool ShouldRollerCommenceAutomation();
    bool GetOperatorThrottle();

    // shifting
    bool ShouldShiftLow();
    bool ShouldShiftHigh();

    // encoder data collection
    bool ShouldCollectEncoderData();

    bool ShouldLoadConfig();
    bool ShouldSaveConfig();
    bool ShouldApplyConfig();

    void UpdateDebouncing();
};

#endif
