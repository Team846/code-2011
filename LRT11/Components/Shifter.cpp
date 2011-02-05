#include "Shifter.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\AsynchronousPrinter.h"
#include <cmath>

Shifter::Shifter()
    : leftShiftServo(RobotConfig::LEFT_GEARBOX_SERVO_PORT)
    , rightShiftServo(RobotConfig::RIGHT_GEARBOX_SERVO_PORT)
    , currentGear(kHighGear)
    , tickCounter(0)
{

}

Shifter::~Shifter()
{

}

void Shifter::Output()
{
    bool leftEngaged, rightEngaged;
    switch(action.Gearbox.gear)
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

    if(currentGear != newGear) //force a shift right away
        tickCounter = 0;

    //don't pulse servo when you haven't engaged

    if(tickCounter++ % (onPulseLength + offPulseLength) >= onPulseLength)
    {
        if(leftEngaged)
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
        AsynchronousPrinter::Printf("Actiondata gear val not set\n");
}
