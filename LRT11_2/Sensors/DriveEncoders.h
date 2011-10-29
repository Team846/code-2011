#ifndef DRIVE_ENCODERS_H_
#define DRIVE_ENCODERS_H_

#include "..\General.h"
#include "LRTEncoder.h"
#include "VirtualLRTEncoder.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\Util.h"
#include "..\Util\AsynchronousPrinter.h"

#define CHANGEME 0

class DriveEncoders
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
    
    double RawForwardSpeed();
#endif

    bool highGear;

public:
    static DriveEncoders& GetInstance();
    virtual ~DriveEncoders();

    const static float PULSES_PER_REVOLUTION = 100.0; // pulses per wheel revolution [BA]

    const static float MAX_ENCODER_RATE      = 1475.0;
    const static float MAX_TURNING_RATE      = 2950.0;

    // 15 degrees short at SVR -KV 3/31/11
    const static float TICKS_PER_FULL_TURN   = 1350.0 * 180.0 / 165.0;
    const static float WHEEL_DIAMETER = 4.0; // in

    // extrapolate max low gear speed
    const static float LOW_GEAR_MULTIPLIER = 16.3 / 6.4;


    const static float PI = 3.14159;

    void SetHighGear(bool isHighGear);

    double ForwardSpeed();
    double NormalizedForwardSpeed();
    double NormalizedForwardMotorSpeed();
    double NormalizedLowGearForwardSpeed();
    double NormalizedLowGearTurningSpeed();
    double TurningSpeed();
    double NormalizedTurningSpeed();
    double NormalizedTurningMotorSpeed();
    
    double NormalizedOpposingGearMotorSpeed(LRTEncoder& encoder);

    double RobotDist();
    int TurnTicks();
    double TurnRevolutions();
    double TurnAngle();
    double WheelDist(int side);
    //    double GetLeftWheelDist();
//   double GetRightWheelDist();
    double LeftSpeed();


//    double GetNormalizedLeftSpeed();
    //speed of the motor if it were engaged
//    double GetNormalizedLeftMotorSpeed();
    double NormalizedLeftOppositeGearMotorSpeed();
    double RightSpeed();
//    double GetNormalizedRightSpeed();
//    double GetNormalizedRightMotorSpeed();
    double NormalizedRightOppositeGearMotorSpeed();

    double NormalizedMotorSpeed(LRTEncoder& encoder);

    bool isHighGear();
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
