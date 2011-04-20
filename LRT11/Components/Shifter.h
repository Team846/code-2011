#ifndef SHIFTER_H_
#define SHIFTER_H_

#include "..\General.h"
#include "Component.h"
#include "Shifter\LRTServo.h"
#include "Shifter\VirtualLRTServo.h"
#include "..\Config\RobotConfig.h"
#include "..\Sensors\DriveEncoders.h"
#include "..\Config\Config.h"

class Shifter : public Component, public Configurable
{
private:
#ifdef VIRTUAL
    VirtualLRTServo leftServo, rightServo;
#else
    LRTServo leftServo, rightServo;
#endif

    DriveEncoders& encoders;

    string prefix;
    enum {LOW_GEAR = 1 , HIGH_GEAR = 2} currentGear, newGear;

    float leftSetpointDeadband;
    float rightSetpointDeadband;

    int forceShiftMs;
    int cycleCounter;

    const static int onPulseLength = 2;
    const static int offPulseLength = 1;

//    const static float leftLowGearServoVal = 0.33;
//    const static float leftHighGearServoVal = 0.62;
    const static float leftLowGearServoVal = 0.62;
    const static float leftHighGearServoVal = 0.33;

    const static float rightLowGearServoVal = 0.67;
    const static float rightHighGearServoVal = 0.39;

    const static float shifterEngagedMargin = 0.05;

public:
    Shifter();
    virtual ~Shifter();

    void virtual Output();
    virtual void Configure();
};

#endif //SHIFTER_H_
