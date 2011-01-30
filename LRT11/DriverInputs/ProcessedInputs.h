#ifndef PROCESSED_JOYSTICK_H_
#define PROCESSED_JOYSTICK_H_

#include "..\General.h"
#include "DebouncedJoystick.h"
#include "..\Config\DriverStationConfig.h"
#include "..\Config\Config.h"
#include "..\Config\Configurable.h"
#include "..\Util\Util.h"

class ProcessedInputs : public SensorBase,
    public Configurable
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

    bool IsShiftPressed();

    float GetForward();
    float GetTurn();

    bool GetBrakeLeft();
    bool GetBrakeRight();

    bool GetServiceMode();

    bool ShouldLCDScrollUp();
    bool ShouldLCDScrollDown();

    void UpdateDebouncing();
};

#endif
