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

    //only let servo become disabled after a delay.
    //when Hitec HS322 servo is disabled, it jumps from it's last set point. -dg
    int servoDisableTimer;
    static const int kServoDisableDelay = 5 * 50; // 5 sec

#ifdef LRT_ROBOT_2011
    //2011 robot
    const static float leftLowGearServoVal = 0.25; // = 0.33;
    const static float leftHighGearServoVal = 0.25 + 0.29; //= 0.62;

    const static float rightLowGearServoVal = 0.67;
    const static float rightHighGearServoVal = 0.39;
#else
    //2010 robot
    const static float leftLowGearServoVal = 0.85; //.62
    const static float leftHighGearServoVal = 0.36; //.33

    const static float rightLowGearServoVal = 0.67;
    const static float rightHighGearServoVal = 0.30;
#endif //LRT_ROBOT_2011 - servo endpoints

public:
    Shifter();
    virtual ~Shifter();

    void virtual Output();
    virtual void Configure();
};

#endif //SHIFTER_H_
