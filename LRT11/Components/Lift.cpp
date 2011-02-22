#include "Lift.h"
#include "..\Config\RobotConfig.h"

Lift::Lift()
    : config(Config::GetInstance())
    , prefix("Lift.")
    , liftEsc(RobotConfig::CAN_LIFT)
    , timeoutMs(0)
    , cycleCount(0)
//    , safety(false)
{

}

Lift::~Lift()
{

}

void Lift::Configure()
{
    liftEsc.SetControlMode(CANJaguar::kPosition);
//    liftEsc.ChangeControlMode(CANJaguar::kPosition);
    liftEsc.SetPositionReference(CANJaguar::kPosRef_Potentiometer);

    liftEsc.SetPID(config.Get<double>(prefix + "pGain", 100), config.Get<double>(prefix + "iGain", 0),
            config.Get<double>(prefix + "dGain", 0));

//    liftEsc.ConfigSoftPositionLimits(config.Get<double>(prefix + "forwardLimit", 0),
//            config.Get<double>(prefix + "reverseLimit", 10));
    liftEsc.SetPotentiometerTurns(10);
//    liftEsc.ConfigPotentiometerTurns(10);

    liftEsc.EnableControl();

    Config& config = Config::GetInstance();
    timeoutMs = config.Get<int>(prefix + "timeoutMs", 1500);

    safetyDeadbandInches = config.Get<float>(prefix + "safetyDeadbandInches", 2);
    safetyMarginInches = config.Get<float>(prefix + "safetyMarginInches", 6);
}

void Lift::Output()
{
    {
        ProfiledSection ps("Lift log position");
        SmartDashboard::Log(liftEsc.GetPosition(), "Lift Position");
    }

    if(!action.lift.givenCommand && cycleCount == 0)
    {
        {
            ProfiledSection ps("Lift disable control");
            liftEsc.DisableControl();
        }
        return;
    }
    else if(cycleCount == 0 || action.lift.givenCommand)
    {
        action.lift.givenCommand = false;
        StartTimer();

        liftEsc.EnableControl();
    }

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
    cycleCount--;

    SmartDashboard::Log(setPoint, "Lift Set Point");
}

void Lift::StartTimer()
{
    cycleCount = (int)(timeoutMs * 1.0 / 1000 * 50.0 / 1);
}
