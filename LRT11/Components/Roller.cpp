#include "Roller.h"
#include "..\Config\RobotConfig.h"
Roller::Roller()
    : topRoller(RobotConfig::CAN_ROLLER_TOP) // change port number later
    , bottomRoller(RobotConfig::CAN_ROLLER_BOTTOM)
    , config(Config::GetInstance())
{
    topRoller.SetControlMode(CANJaguar::kCurrent);
    bottomRoller.SetControlMode(CANJaguar::kCurrent);
}

Roller::~Roller()
{

}

void Roller::RollInward()
{
    topRoller.Set(currentSuckingIn);
    bottomRoller.Set(currentSuckingIn);
}

void Roller::RollOutward()
{
    topRoller.Set(currentSpittingOut);
    bottomRoller.Set(currentSpittingOut);
}

void Roller::Stop()
{
    topRoller.Set(0.0);
    bottomRoller.Set(0.0);
}

void Roller::RollOpposite(int direction)
{
    topRoller.Set(Util::Sign<int>(direction) * 1.0);
    bottomRoller.Set(Util::Sign<int>(direction) * -1.0);
}

void Roller::Output()
{
    switch(action.roller.state)
    {
    case STOPPED:
        Stop();
        break;
    case SUCKING:
        RollInward();
        break;
    case SPITTING:
        RollOutward();
        break;
    case ROLLING:
        // TODO
        break;
    }
}

void Roller::Configure()
{
    const static string prefix = "Roller.";
    currentSpittingOut = config.Get<float>(prefix + "currentSpittingOut");
    currentSuckingIn = config.Get<float>(prefix + "currentSuckingIn");
}
