#include "Lift.h"
#include "..\Config\RobotConfig.h"

Lift::Lift()
    : config(Config::GetInstance())
    , prefix("Lift.")
    , liftEsc(RobotConfig::CAN_LIFT, 10, prefix)
{

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
    case STOWED:
        key += "stowed";
        break;
    case LOW_PEG:
        key += "lowPeg";
        break;
    case MED_PEG:
        key += "mediumPeg";
        break;
    case HIGH_PEG:
        key += "highPeg";
        break;
    }

    float setPoint = config.Get<float>(key);
    liftEsc.Set(setPoint);

    SmartDashboard::Log(setPoint, "Lift Set Point");
    SmartDashboard::Log(liftEsc.GetPosition(), "Lift Position");
}
