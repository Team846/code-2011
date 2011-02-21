#ifndef DRIVE_ENCODERS_H_
#define DRIVE_ENCODERS_H_

#include "..\General.h"
#include "LRTEncoder.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\Util.h"
#include "..\Util\AsynchronousPrinter.h"

#define CHANGEME 0

class DriveEncoders : public SensorBase
{
private:
    static DriveEncoders* instance;

    // encoderLeftUseless and encoderRightUseless used to fix WPILib bug;
    // only evenly instantiated encoders work
    LRTEncoder encoderLeft;
    LRTEncoder encoderRight;

    // floats to prevent integer division
    const static float kWheelDiameter = 8.0; // in

    // output sprocket:wheel sprocket is 36:48
    const static float kPi          = 3.14159;
    const static float kTrackLength = 23.0; // in

public:
    static DriveEncoders& GetInstance();
    virtual ~DriveEncoders();

    const static float kPulsesPerRevolution = 100.0; // pulses per wheel revolution [BA]
//    const static float kMaxEncoderRate      = 1411.96;
    const static float kMaxEncoderRate      = 1400.0;
    const static double kMaxTurningRate     = 2800.0;
//    const static double kTicksPerFullTurn   = 6725. / 6;
    const static double kTicksPerFullTurn   = 1350.0;

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
    double GetNormalizedLeftSpeed();
    double GetNormalizedRightSpeed();

    LRTEncoder& GetLeftEncoder();
    LRTEncoder& GetRightEncoder();

protected:
    DriveEncoders();
    DISALLOW_COPY_AND_ASSIGN(DriveEncoders);
};

#endif
