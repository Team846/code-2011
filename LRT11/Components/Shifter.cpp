#include "Shifter.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\AsynchronousPrinter.h"

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
    switch(action.Gearbox.gear)
    {
    case kLowGear :
        newGear = kLowGear;
    case kHighGear :
        newGear = kHighGear;
    }

    if(currentGear != newGear) //force a shift right away
        tickCounter = 0;

    if(tickCounter++ % (onPulseLength + offPulseLength) >= onPulseLength)
    {
        leftShiftServo.SetOffline();
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
