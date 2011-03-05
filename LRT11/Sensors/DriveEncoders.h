#ifndef DRIVE_ENCODERS_H_
#define DRIVE_ENCODERS_H_

#include "..\General.h"
#include "VirtualLRTEncoder.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\Util.h"
#include "..\Util\AsynchronousPrinter.h"

#define CHANGEME 0

class DriveEncoders : public SensorBase
{
private:
    static DriveEncoders* instance;

    VirtualLRTEncoder encoderLeft;
    VirtualLRTEncoder encoderRight;

public:
    static DriveEncoders& GetInstance();
    virtual ~DriveEncoders();

    const static float PULSES_PER_REVOLUTION = 100.0; // pulses per wheel revolution [BA]
//    const static float kMaxEncoderRate      = 1411.96;
    const static float MAX_ENCODER_RATE      = 1400.0;
    const static double MAX_TURNING_RATE     = 2800.0;
//    const static double kTicksPerFullTurn   = 6725. / 6;
    const static double TICKS_PER_FULL_TURN   = 1350.0;

    const static float WHEEL_DIAMETER = 8.0; // in
    const static float PI = 3.14159;

    double GetForwardSpeed();
    double GetNormalizedForwardSpeed();
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

    VirtualLRTEncoder& GetLeftEncoder();
    VirtualLRTEncoder& GetRightEncoder();

protected:
    DriveEncoders();
    DISALLOW_COPY_AND_ASSIGN(DriveEncoders);
};

#endif
