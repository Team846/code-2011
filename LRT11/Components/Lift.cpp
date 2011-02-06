#include "Lift.h"
#include "..\Config\RobotConfig.h"

Lift::Lift()
    : config(Config::GetInstance())
    , prefix("lift.")
    , liftEsc(RobotConfig::CAN_LIFT)
{
    liftEsc.SetControlMode(CANJaguar::kPosition);
    liftEsc.SetPositionReference(CANJaguar::kPosRef_Potentiometer);
    liftEsc.SetPID(config.Get<double>(prefix + "PGain"), config.Get<double>(prefix + "IGain"), config.Get<double>(prefix + "DGain"));
    liftEsc.EnableControl();
}

Lift::~Lift()
{

}

void Lift::Output()
{
    string key = prefix;
    if(action.lift.highRow)
        key += "high.";
    else
        key += "low.";

    switch(action.lift.position)
    {
    case kStowed:
        key += "stowed";
        break;
    case kLowPeg:
        key += "lowPeg";
        break;
    case kMedPeg:
        key += "medPeg";
        break;
    case kHighPeg:
        key += "highPeg";
        break;
    }

    float setPoint = config.Get<float>(key);
    liftEsc.Set(setPoint);

    SmartDashboard::Log(setPoint, "LiftSetPoint");
    SmartDashboard::Log(liftEsc.GetPosition(), "LiftPosition");

}

