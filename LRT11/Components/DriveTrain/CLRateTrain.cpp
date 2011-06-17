#include "CLRateTrain.h"

CLRateTrain::CLRateTrain()
//    : DriveMethod(escLeft, escRight)
//    , escLeft(escLeft)
//    , escRight(escRight)
    : encoders(DriveEncoders::GetInstance())
    , config(Config::GetInstance())
    , fwdRunningError(FWD_DECAY)
    , turnRunningError(TURN_DECAY)
    , brakeLeft(false)
    , brakeRight(false)
    , usingClosedLoop(true)
    , highGear(true)
{
    printf("Constructed CLRateTrain\n");
}

void CLRateTrain::Configure()
{
    // confiure parent class
    DitheredBrakeTrain::Configure();

    const static string configSection = "CLRateDriveTrain";

    pGainTurnHighGear = config.Get<float>(configSection, "pGainTurnHighGear", 1.5);
    pGainFwdHighGear = config.Get<float>(configSection, "pGainFwdHighGear", 1.5);

    pGainTurnLowGear = config.Get<float>(configSection, "pGainTurnLowGear", 1.5);
    pGainFwdLowGear = config.Get<float>(configSection, "pGainFwdLowGear", 1.5);
}

DriveCommand CLRateTrain::Drive(float rawFwd, float rawTurn)
{
    if(brakeLeft && brakeRight)
        Stop();
    else if(brakeLeft)
        PivotLeft(rawFwd);
    else if(brakeRight)
        PivotRight(rawFwd);
    // TODO FIX
//    if(brakeLeft || brakeRight)
//        return;

    if(!usingClosedLoop)
    {
#ifdef USE_DASHBOARD
//        SmartDashboard::Log(rawFwd, "Raw Forward (CLDT)");
//        SmartDashboard::Log(rawTurn, "Raw Turn (CLDT)");
#endif
        return DitheredBrakeTrain::Drive(rawFwd, rawTurn);
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

    float robotSpeed = encoders.GetNormalizedForwardMotorSpeed();
    // don't want to limit the top speed of the drivetrain
    robotSpeed = Util::Clamp<float>(robotSpeed, -1.0, 1.0);

    float fwdError = rawFwd - robotSpeed;
    fwdError = fwdRunningError.UpdateSum(fwdError);

    float fwdCorrection = fwdError * pGainFwd;
    float newFwd = rawFwd + fwdCorrection;

#ifdef USE_DASHBOARD
//    SmartDashboard::Log(turningRate, "Turning Rate");
    SmartDashboard::Log(pGainFwd, "Forward Gain");
    SmartDashboard::Log(pGainTurn, "Turn Gain");
//    SmartDashboard::Log(rawFwd, "Raw Forward (CLDT)");
//    SmartDashboard::Log(rawTurn, "Raw Turn (CLDT)");
//    SmartDashboard::Log(newFwd, "Forward");
//    SmartDashboard::Log(newTurn, "Turn");
#endif

    return DitheredBrakeTrain::Drive(newFwd, newTurn);
}

void CLRateTrain::PivotLeft(float rightSpeed)
{
//    escLeft.Stop();
//    escRight.Set(rightSpeed);
}

void CLRateTrain::PivotRight(float leftSpeed)
{
//    escRight.Stop();
//    escLeft.Set(leftSpeed);
}

void CLRateTrain::SetBrakeLeft(bool brakeLeft)
{
    this->brakeLeft = brakeLeft;
}

void CLRateTrain::SetBrakeRight(bool brakeRight)
{
    this->brakeRight = brakeRight;
}

void CLRateTrain::Stop()
{
//    escRight.Stop();
//    escLeft.Stop();
}

void CLRateTrain::SetClosedLoopEnabled(bool enabled)
{
    usingClosedLoop = enabled;
}

void CLRateTrain::SetHighGear(bool isHighGear)
{
    highGear = isHighGear;
}
