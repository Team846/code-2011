#include "LRTServo.h"

LRTServo::LRTServo(UINT32 channel, char* name)
    : Servo(channel)
    , enabled(true)
    , name(name)
{
    if(!name) name = "servo";
    printf("Created %s on channel %d\n", name, channel);
}

LRTServo::~LRTServo()
{
    // nothing to do
}

void LRTServo::SetEnabled(bool enabled)
{
    if(this->enabled != enabled)  //state change; TODO delete this.
        printf("Servo: %d\n", enabled);

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
