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

    float setPoint;
    if(action.lift.highRow)
        setPoint = config.Get<float>(prefix + "highRowBottom");
    else
        setPoint = config.Get<float>(prefix + "lowRowBottom");

    switch(action.lift.position)
    {
    case STOWED:
        break; // no relative position
    case LOW_PEG:
        key += "lowPegRelative";
        break;
    case MED_PEG:
        key += "mediumPegRelative";
        break;
    case HIGH_PEG:
        key += "highPegRelative";
        break;
    }

    if(action.lift.position != action.lift.STOWED)
        setPoint += config.Get<float>(key); // relative to bottom

    liftEsc.Set(setPoint);

    SmartDashboard::Log(setPoint, "Lift Set Point");
    SmartDashboard::Log(liftEsc.GetPosition(), "Lift Position");
}
