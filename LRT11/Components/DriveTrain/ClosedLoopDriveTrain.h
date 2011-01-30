#ifndef CLOSED_LOOP_DRIVE_TRAIN_H_
#define CLOSED_LOOP_DRIVE_TRAIN_H_

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Jaguar\Esc.h"
#include "DriveMethod.h"
#include "DBSDrive.h"

class RunningSum
{
public:
    RunningSum(float decayConstant) :
        decayConstant(decayConstant), runningSum(0)
    {
    }

    float UpdateSum(float x)
    {
        runningSum *= decayConstant;
        runningSum += x;
        return runningSum * (1 - decayConstant);
    }

private:
    float decayConstant;
    float runningSum;
};

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

    const static float kFwdDecay = 0.5;
    const static float kTurnDecay = 0.5;

};

#endif
