#include "Shifter.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\AsynchronousPrinter.h"
#include <cmath>

Shifter::Shifter()
    : leftShiftServo(RobotConfig::LEFT_GEARBOX_SERVO_PORT)
    , rightShiftServo(RobotConfig::RIGHT_GEARBOX_SERVO_PORT)
    , currentGear(kLowGear)
    , tickCounter(0)
{

}

Shifter::~Shifter()
{

}

void Shifter::Output()
{
    bool leftEngaged, rightEngaged;
    switch(action.gearbox.gear)
    {
    case kLowGear :
        newGear = kLowGear;

        leftEngaged = fabs(leftShiftServo.Get() - leftLowGearServoVal)
                < shifterEngagedMargin;
        rightEngaged = fabs(rightShiftServo.Get() - rightLowGearServoVal)
                < shifterEngagedMargin;

        break;
    case kHighGear :
        newGear = kHighGear;

        leftEngaged = fabs(leftShiftServo.Get() - leftHighGearServoVal)
                < shifterEngagedMargin;
        rightEngaged = fabs(rightShiftServo.Get() - rightHighGearServoVal)
                < shifterEngagedMargin;

        break;
    }

    if(tickCounter++ % (onPulseLength + offPulseLength) >= onPulseLength)
    {
        if(leftEngaged) //don't turn off the servo unless you are engaged
            leftShiftServo.SetOffline();
        if(rightEngaged)
            rightShiftServo.SetOffline();
        return;
    }

    if(currentGear == kLowGear)
    {
        leftShiftServo.Set(leftLowGearServoVal);
        rightShiftServo.Set(rightLowGearServoVal);
    }
    else if(currentGear == kHighGear)
    {
        leftShiftServo.Set(leftHighGearServoVal);
        rightShiftServo.Set(rightHighGearServoVal);
    }
    else
        AsynchronousPrinter::Printf("ActionData gearbox.gear value has not been set.\n");
}
