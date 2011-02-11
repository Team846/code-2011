#include "ClosedLoopDriveTrain.h"

ClosedLoopDriveTrain::ClosedLoopDriveTrain(Esc& escLeft, Esc& escRight,
        DriveEncoders& encoders, DBSDrive& dbsDrive)
    : DriveMethod(escLeft, escRight)
    , escLeft(escLeft)
    , escRight(escRight)
    , encoders(encoders)
    , config(Config::GetInstance())
    , dbsDrive(dbsDrive)
    , fwdRunningError(FWD_DECAY)
    , turnRunningError(TURN_DECAY)
    , brakeLeft(false)
    , brakeRight(false)
{
    Config::RegisterConfigurable(this);
}

void ClosedLoopDriveTrain::Configure()
{
    const static string prefix = "ClosedLoopDriveTrain.";

    pGainTurn = config.Get<float>(prefix + "pGainTurn", 1.5);
    pGainFwd = config.Get<float>(prefix + "pGainFwd", 1.5);

    fullBrakingThreshold = config.Get<float>(prefix + "fullBrakingThreshold",
            0.75);
    maxSpeedReversePower = config.Get<float>(prefix + "maxSpeedReversePower",
            0.3);
    driveStraightTurningTolerance = config.Get<float>(prefix
            + "driveStraightTurningTolerance", 0.15);
}

DriveOutput ClosedLoopDriveTrain::ComputeArcadeDrive(float rawFwd,
        float rawTurn)
{
    if(brakeLeft && brakeRight)
        Stop();
    else if(brakeLeft)
        PivotLeft(rawFwd);
    else if(brakeRight)
        PivotRight(rawFwd);
    if(brakeLeft || brakeRight)
        return NO_OUTPUT;

    float turningRate = encoders.GetNormalizedTurningSpeed();

    // eliminate spurrious measurements above mag |1|
    // values over mag |1| will cause the closed loop to slow down
    turningRate = Util::Clamp<float>(turningRate, -1, 1);

    // update the running sum with the error
    float turningError = rawTurn - turningRate;
    turningError = turnRunningError.UpdateSum(turningError);

    float turningCorrection = turningError * pGainTurn;
    float newTurn = rawTurn + turningCorrection;

    // normalized forward speed
    float robotSpeed = encoders.GetNormalizedForwardSpeed();

    float fwdError = rawFwd - robotSpeed;
    fwdError = fwdRunningError.UpdateSum(fwdError);

    float fwdCorrection = fwdError * pGainFwd;
    float newFwd = rawFwd + fwdCorrection;

    SmartDashboard::Log(turningRate, "Turning Rate");
    SmartDashboard::Log(robotSpeed, "Robot Speed");
    SmartDashboard::Log(pGainFwd, "Forward Gain");
    SmartDashboard::Log(pGainTurn, "Turn Gain");
    SmartDashboard::Log(newFwd, "Forward");
    SmartDashboard::Log(newTurn, "Turn");

    return dbsDrive.ComputeArcadeDrive(newFwd, newTurn);
}

void ClosedLoopDriveTrain::PivotLeft(float rightSpeed)
{
    escLeft.Stop();
    escRight.Set(rightSpeed);
}

void ClosedLoopDriveTrain::PivotRight(float leftSpeed)
{
    escRight.Stop();
    escLeft.Set(leftSpeed);
}

void ClosedLoopDriveTrain::SetBrakeLeft(bool brakeLeft)
{
    this->brakeLeft = brakeLeft;
}

void ClosedLoopDriveTrain::SetBrakeRight(bool brakeRight)
{
    this->brakeRight = brakeRight;
}

void ClosedLoopDriveTrain::Stop()
{
    escRight.Stop();
    escLeft.Stop();
}
