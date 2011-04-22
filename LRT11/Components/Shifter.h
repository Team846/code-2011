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

//    float leftSetpointDeadband;
//    float rightSetpointDeadband;

    int kShiftTime;	//Cycles reserved for shifting.  From config file.
    int cycleCounter;
    
    //only let servo become disabled after a delay.
    //when Hitec HS322 servo is disabled, it jumps from it's last set point. -dg
    int servoDisableTimer;
    static const int kServoDisableDelay = 5 * 50; // 5 sec
 
//    const static int onPulseLength = 2;
//    const static int offPulseLength = 1;


#if 0
    //2011 robot
     const static float leftLowGearServoVal = 0.33;
     const static float leftHighGearServoVal = 0.62;
     const static float rightLowGearServoVal = 0.67;
     const static float rightHighGearServoVal = 0.39;
#else
    //2010 robot
#warning "Servo Settings for 2010 robot - not 2011 robot!"
    const static float leftLowGearServoVal = 0.85; //.62
    const static float leftHighGearServoVal = 0.36; //.33

    const static float rightLowGearServoVal = 0.67;
    const static float rightHighGearServoVal = 0.30;
#endif //servo endpoints
 //   const static float shifterEngagedMargin = 0.05;  //unknown purpose -dg

public:
    Shifter();
    virtual ~Shifter();

    void virtual Output();
    virtual void Configure();
};

#endif //SHIFTER_H_
