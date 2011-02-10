#include "Lift.h"
#include "..\Config\RobotConfig.h"

Lift::Lift()
    : config(Config::GetInstance())
    , prefix("Lift.")
    , liftEsc(RobotConfig::CAN_LIFT, 10, prefix)
{
    liftEsc.LoadSoftPositionLimits(0, 10);
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
        key += "mediumPeg";
        break;
    case kHighPeg:
        key += "highPeg";
        break;
    }

    float setPoint = config.Get<float>(key);
    liftEsc.Set(setPoint);

    SmartDashboard::Log(setPoint, "Lift Set Point");
    SmartDashboard::Log(liftEsc.GetPosition(), "Lift Position");
}