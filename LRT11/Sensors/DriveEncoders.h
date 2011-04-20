#ifndef DRIVE_ENCODERS_H_
#define DRIVE_ENCODERS_H_

#include "..\General.h"
#include "LRTEncoder.h"
#include "VirtualLRTEncoder.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\Util.h"
#include "..\Util\AsynchronousPrinter.h"

#define CHANGEME 0

class DriveEncoders : public SensorBase
{
private:
    static DriveEncoders* instance;
#ifdef VIRTUAL
    VirtualLRTEncoder encoderLeft;
    VirtualLRTEncoder encoderRight;
#else
    LRTEncoder encoderLeft;
    LRTEncoder uselessEncoder;
    LRTEncoder encoderRight;
#endif

    bool isHighGear;

public:
    static DriveEncoders& GetInstance();
    virtual ~DriveEncoders();

#ifdef LRT_ROBOT_2011
    const static float PULSES_PER_REVOLUTION = 100.0; // pulses per wheel revolution [BA]

    const static float MAX_ENCODER_RATE      = 1400.0;
    const static float MAX_TURNING_RATE      = 2800.0;

    // 15 degrees short at SVR -KV 3/31/11
    const static float TICKS_PER_FULL_TURN   = 1350.0 * 180.0 / 165.0;
    const static float WHEEL_DIAMETER = 4.0; // in

    // extrapolate max low gear speed
    const static float LOW_GEAR_MULTIPLIER = 16.3 / 6.4;
#else
    const static float PULSES_PER_REVOLUTION = 187.5; // pulses per wheel revolution [BA]

    const static float MAX_ENCODER_RATE      = 1800;
    const static float MAX_TURNING_RATE      = 448.3;

    const static float TICKS_PER_FULL_TURN   = 997.0; // measured 3/28/11 at room 612 -KV
    const static float WHEEL_DIAMETER        = 8.0; // in
#endif

    // extrapolate max low gear speed
    const static float LOW_GEAR_MULTIPLIER = 3 / 1.35 / 0.83;

    const static float PI = 3.14159;

    void SetHighGear(bool isHighGear);

    double GetForwardSpeed();
    double GetNormalizedForwardSpeed();
    double GetNormalizedForwardMotorSpeed();
    double GetNormalizedLowGearForwardSpeed();
    double GetNormalizedLowGearTurningSpeed();
    double GetTurningSpeed();
    double GetNormalizedTurningSpeed();
    double GetNormalizedMotorTurningSpeed();
    double GetRobotDist();
    int GetTurnTicks();
    double GetTurnRevolutions();
    double GetTurnAngle();
    double GetLeftWheelDist();
    double GetRightWheelDist();
    double GetLeftSpeed();
    double GetNormalizedLeftSpeed();
    
    //speed of the motor if it were engaged
    double GetNormalizedLeftMotorSpeed();
    double GetNormalizedLeftOppositeGearMotorSpeed();
    double GetRightSpeed();
    double GetNormalizedRightSpeed();
    double GetNormalizedRightMotorSpeed();
    double GetNormalizedRightOppositeGearMotorSpeed();

#ifdef VIRTUAL
    VirtualLRTEncoder& GetLeftEncoder();
    VirtualLRTEncoder& GetRightEncoder();
#else
    LRTEncoder& GetLeftEncoder();
    LRTEncoder& GetRightEncoder();
#endif

protected:
    DriveEncoders();
    DISALLOW_COPY_AND_ASSIGN(DriveEncoders);
};

#endif
