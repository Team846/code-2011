#ifndef CL_POSITION_DRIVE_TRAIN_H_
#define CL_POSITION_DRIVE_TRAIN_H_

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Util\RunningSum.h"
#include "CLRateDriveTrain.h"

class CLPositionDriveTrain : public Configurable
{
public:
    CLPositionDriveTrain(CLRateDriveTrain train);

    virtual void Configure();

    void MoveInches(float inches);

    // positive is CW; negative is CCW
    void TurnAngle(float angle, bool pivotLeft = false, bool pivotRight = false);

    // true returned if maneuver is finished (error < deadband)
    bool MoveDistanceOutput();
    bool TurnAngleOutput();

    void Stop();

    void SetClosedLoopEnabled(bool enabled);

private:
    CLRateDriveTrain drive;
    DriveEncoders& encoders;

    struct
    {
        bool hasCommand;
        float target;
        float delta;
    } moveDistanceInfo;

    struct
    {
        bool hasCommand;
        bool pivotLeft, pivotRight;
        float target, distance;
    } turnAngleInfo;

    float pGainFwd;
    float pGainFwdTurnCorrection;
    float pGainTurn;
    float pGainTurnFwdCorrection;

    float fwdDeadband;
    float turnDeadband;

    RunningSum forwardRunningError;
    RunningSum turnRunningError;

    const static float FWD_DECAY = 0.5;
    const static float TURN_DECAY = 0.5;
};

#endif
