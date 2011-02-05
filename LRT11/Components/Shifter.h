#ifndef SHIFTER_H_
#define SHIFTER_H_

#include "Component.h"
#include "LRTServo.h"

class Shifter : Component
{
private:
    LRTServo leftShiftServo, rightShiftServo;
    enum {kLowGear = 1 , kHighGear = 2} currentGear, newGear;

    int tickCounter;

    const static int onPulseLength = 2;
    const static int offPulseLength = 1;

#warning "Gearbox servo positions not set for 2011 robot"  
    const static float leftHighGearServoVal = 0.35;
    const static float leftLowGearServoVal = 0.65;

    const static float rightHighGearServoVal = 0.3;
    const static float rightLowGearServoVal = 0.65;

public:
    Shifter();
    virtual ~Shifter();

    void virtual Output();
};

#endif //SHIFTER_H_
