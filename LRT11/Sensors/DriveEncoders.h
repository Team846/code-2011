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

public:
    static DriveEncoders& GetInstance();
    virtual ~DriveEncoders();

    const static float PULSES_PER_REVOLUTION = 100.0; // pulses per wheel revolution [BA]
//    const static float kMaxEncoderRate      = 1411.96;
    const static float MAX_ENCODER_RATE      = 1400.0;
    const static double MAX_TURNING_RATE     = 2800.0;
//    const static double kTicksPerFullTurn   = 6725. / 6;
    const static double TICKS_PER_FULL_TURN   = 1350.0;

    // extrapolate max low gear speed
    const static float LOW_GEAR_MULTIPLIER = 16.3 / 6.4;

    const static float WHEEL_DIAMETER = 4.0; // in
    const static float PI = 3.14159;

    double GetForwardSpeed();
    double GetNormalizedForwardSpeed();
    double GetNormalizedLowGearForwardSpeed();
    double GetNormalizedLowGearTurningSpeed();
    double GetTurningSpeed();
    double GetNormalizedTurningSpeed();
    double GetRobotDist();
    double GetTurnTicks();
    double GetTurnRevolutions();
    double GetTurnAngle();
    double GetLeftWheelDist();
    double GetRightWheelDist();
    double GetLeftSpeed();
    double GetNormalizedLeftSpeed();
    double GetRightSpeed();
    double GetNormalizedRightSpeed();

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
