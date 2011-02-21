#include "LRTServo.h"

LRTServo::LRTServo(UINT32 channel)
    : Servo(channel)
    , enabled(true)
{

}

LRTServo::~LRTServo()
{
    // nothing to do
}

void LRTServo::SetEnabled(bool enabled)
{
    this->enabled = enabled;

    if(!enabled)
        this->SetOffline();
}

bool LRTServo::IsEnabled()
{
    return enabled;
}

void LRTServo::Set(float value)
{
    if(enabled)
        Servo::Set(value);
}

void LRTServo::SetAngle(float angle)
{
    if(enabled)
        Servo::SetAngle(angle);
}
