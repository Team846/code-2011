#include "Roller.h"
#include "..\Config\RobotConfig.h"

Roller::Roller()
    : topRoller(RobotConfig::CAN_ROLLER_TOP) // change port number later
    , bottomRoller(RobotConfig::CAN_ROLLER_BOTTOM)
    , prefix("Roller.")
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
    int sign = Util::Sign<int>(direction);
    topRoller.Set(sign * 1.0);
    bottomRoller.Set(sign * -1.0);
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
    Config& config = Config::GetInstance();

    currentSpittingOut = config.Get<float>(prefix + "currentSpittingOut");
    currentSuckingIn = config.Get<float>(prefix + "currentSuckingIn");
}
