#include "CLRateDriveTrain.h"

CLRateDriveTrain::CLRateDriveTrain(Esc& escLeft, Esc& escRight,
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
    , usingClosedLoop(true)
    , highGear(true)
{
}

void CLRateDriveTrain::Configure()
{
    const static string prefix = "CLRateDriveTrain.";

    pGainTurnHighGear = config.Get<float>(prefix + "pGainTurnHighGear", 1.5);
    pGainFwdHighGear = config.Get<float>(prefix + "pGainFwdHighGear", 1.5);

    pGainTurnLowGear = config.Get<float>(prefix + "pGainTurnLowGear", 1.5);
    pGainFwdLowGear = config.Get<float>(prefix + "pGainFwdLowGear", 1.5);
}

DriveOutput CLRateDriveTrain::ComputeArcadeDrive(float rawFwd,
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

    if(!usingClosedLoop)
    {
#ifdef USE_DASHBOARD
//        SmartDashboard::Log(rawFwd, "Raw Forward (CLDT)");
//        SmartDashboard::Log(rawTurn, "Raw Turn (CLDT)");
#endif
        return dbsDrive.ComputeArcadeDrive(rawFwd, rawTurn);
    }

    float pGainTurn = highGear ? pGainTurnHighGear : pGainTurnLowGear;
    float pGainFwd = highGear ? pGainFwdHighGear : pGainFwdLowGear;

    float turningRate = highGear ? encoders.GetNormalizedTurningSpeed()
			: encoders.GetNormalizedLowGearTurningSpeed();

    // eliminate spurrious measurements above mag |1|
    // values over mag |1| will cause the closed loop to slow down
    turningRate = Util::Clamp<float>(turningRate, -1, 1);

    // update the running sum witrah the error
    float turningError = rawTurn - turningRate;
    turningError = turnRunningError.UpdateSum(turningError);

    float turningCorrection = turningError * pGainTurn;
    float newTurn = rawTurn + turningCorrection;

    // normalized forward speed
    float robotSpeed = highGear ? encoders.GetNormalizedForwardSpeed()
			: encoders.GetNormalizedLowGearForwardSpeed();

    float fwdError = rawFwd - robotSpeed;
    fwdError = fwdRunningError.UpdateSum(fwdError);

    float fwdCorrection = fwdError * pGainFwd;
    float newFwd = rawFwd + fwdCorrection;

#ifdef USE_DASHBOARD
    //SmartDashboard::Log(turningRate, "Turning Rate");
//    SmartDashboard::Log(pGainFwd, "Forward Gain");
//    SmartDashboard::Log(pGainTurn, "Turn Gain");
//    SmartDashboard::Log(rawFwd, "Raw Forward (CLDT)");
//    SmartDashboard::Log(rawTurn, "Raw Turn (CLDT)");
//    SmartDashboard::Log(newFwd, "Forward");
//    SmartDashboard::Log(newTurn, "Turn");
#endif

    return dbsDrive.ComputeArcadeDrive(newFwd, newTurn);
}

void CLRateDriveTrain::PivotLeft(float rightSpeed)
{
    escLeft.Stop();
    escRight.Set(rightSpeed);
}

void CLRateDriveTrain::PivotRight(float leftSpeed)
{
    escRight.Stop();
    escLeft.Set(leftSpeed);
}

void CLRateDriveTrain::SetBrakeLeft(bool brakeLeft)
{
    this->brakeLeft = brakeLeft;
}

void CLRateDriveTrain::SetBrakeRight(bool brakeRight)
{
    this->brakeRight = brakeRight;
}

void CLRateDriveTrain::Stop()
{
    escRight.Stop();
    escLeft.Stop();
}

void CLRateDriveTrain::SetClosedLoopEnabled(bool enabled)
{
    usingClosedLoop = enabled;
}

void CLRateDriveTrain::SetHighGear(bool isHighGear)
{
    highGear = isHighGear;
}
