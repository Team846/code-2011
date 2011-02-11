#ifndef CLOSED_LOOP_DRIVE_TRAIN_H_
#define CLOSED_LOOP_DRIVE_TRAIN_H_

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Jaguar\Esc.h"
#include "..\..\Util\RunningSum.h"
#include "DriveMethod.h"
#include "DBSDrive.h"

class ClosedLoopDriveTrain : public DriveMethod, public Configurable
{
public:
    ClosedLoopDriveTrain(Esc& escLeft, Esc& escRight, DriveEncoders& encoders,
            DBSDrive& dbsDrive);

    virtual DriveOutput ComputeArcadeDrive(float rawFwd, float rawTurn);
    void PivotLeft(float rightSpeed);
    void PivotRight(float leftSpeed);

    void SetBrakeLeft(bool brakeLeft);
    void SetBrakeRight(bool brakeRight);

    void Stop();
    virtual void Configure();

private:
    Esc& escLeft, &escRight;
    DriveEncoders& encoders;

    Config& config;
    DBSDrive& dbsDrive;

    float pGainTurn;
    float pGainFwd;

    float maxSpeedReversePower;
    float fullBrakingThreshold;
    float driveStraightTurningTolerance;

    RunningSum fwdRunningError;
    RunningSum turnRunningError;

    bool brakeLeft;
    bool brakeRight;

    const static float FWD_DECAY = 0.5;
    const static float TURN_DECAY = 0.5;
};

#endif
