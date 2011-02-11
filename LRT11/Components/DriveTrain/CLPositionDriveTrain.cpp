#include "CLPositionDriveTrain.h"

CLPositionDriveTrain::CLPositionDriveTrain(ClosedLoopDriveTrain train)
    : drive(train)
    , encoders(DriveEncoders::GetInstance())
    , forwardRunningError(FWD_DECAY)
    , turnRunningError(TURN_DECAY)
{
    Config::RegisterConfigurable(this);
    moveDistanceInfo.hasCommand = false;
}

void CLPositionDriveTrain::Configure()
{
    Config& config = Config::GetInstance();
    const string prefix = "CLPositionDriveTrain.";

    pGainFwd = config.Get<float>(prefix + "pGainFwd", 1.5);
    pGainTurn = config.Get<float>(prefix + "pGainTurn", 1.5);

    fwdDeadband = config.Get<float>(prefix + "fwdDeadband", 0.05);
    turnDeadband = config.Get<float>(prefix + "turnDeadband", 0.05);
}

void CLPositionDriveTrain::MoveInches(float inches)
{
    float cm = inches * 2.54;
    moveDistanceInfo.target = encoders.GetRobotDist() + cm;
    moveDistanceInfo.distance = cm;
    moveDistanceInfo.hasCommand = true;
}

void CLPositionDriveTrain::TurnAngle(float angle, bool pivotLeft, bool pivotRight)
{
    turnAngleInfo.target = encoders.GetTurnAngle() + angle;
    turnAngleInfo.angle = angle;
    turnAngleInfo.pivotLeft = pivotLeft;
    turnAngleInfo.pivotRight = pivotRight;
    turnAngleInfo.hasCommand = true;
}

bool CLPositionDriveTrain::MoveDistanceOutput()
{
    if(!moveDistanceInfo.hasCommand)
        return true;

    float error = (moveDistanceInfo.target - encoders.GetRobotDist()) / moveDistanceInfo.distance;
    float newError = forwardRunningError.UpdateSum(error);

    // arcade drive assumes inputs are within [-1,1] interval
    float correction = Util::Clamp<float>(newError * pGainFwd, -1, 1);
    drive.ArcadeDrive(correction, 0);

    return Util::Abs<float>(error) < fwdDeadband;
}

bool CLPositionDriveTrain::TurnAngleOutput()
{
    if(!turnAngleInfo.hasCommand)
        return true;

    float error = (turnAngleInfo.target - encoders.GetTurnAngle()) / turnAngleInfo.angle;
    float newError = turnRunningError.UpdateSum(error);

    // arcade drive, pivot left, and pivot right all assume inputs are within [-1,1] interval
    float correction = Util::Clamp<float>(newError * pGainTurn, -1, 1);

    // output based on pivot flags
    if(turnAngleInfo.pivotLeft)
        drive.PivotLeft(correction);
    else if(turnAngleInfo.pivotRight)
        drive.PivotRight(correction);
    else
        drive.ArcadeDrive(0, correction);

    return Util::Abs<float>(error) < turnDeadband;
}
