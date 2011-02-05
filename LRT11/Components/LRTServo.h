#ifndef LRT_SERVO_H
#define LRT_SERVO_H

#include "WPILib.h"

class LRTServo : public Servo
{
public:
    LRTServo(UINT32 channel) :
        Servo(channel) {}

    void TurnOff()
    {
        // SetSpeed is not available with the
        // regular servo class because it is
        // protected - not used anymore
//      SetSpeed(0);

        SetRaw(0);
    }

};
#endif
