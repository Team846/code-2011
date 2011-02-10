#ifndef SHIFTER_H_
#define SHIFTER_H_

#include "Component.h"

class Shifter : public Component
{
private:
    Servo leftShiftServo, rightShiftServo;
    enum {kLowGear = 1 , kHighGear = 2} currentGear, newGear;

    int tickCounter;

    const static int onPulseLength = 2;
    const static int offPulseLength = 1;

    const static float leftHighGearServoVal = 0.35;
    const static float leftLowGearServoVal = 0.65;

    const static float rightHighGearServoVal = 0.3;
    const static float rightLowGearServoVal = 0.65;

    const static float shifterEngagedMargin = 0.05;

public:
    Shifter();
    virtual ~Shifter();

    void virtual Output();
};

#endif //SHIFTER_H_
