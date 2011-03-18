#include "Roller.h"
#include "..\Config\RobotConfig.h"

Roller::Roller()
    : topRoller(RobotConfig::CAN_ROLLER_TOP) // change port numbers later
    , bottomRoller(RobotConfig::CAN_ROLLER_BOTTOM)
    , prefix("Roller.")
{
    topRoller.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
    bottomRoller.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
}

Roller::~Roller()
{

}

void Roller::RollInward()
{
    // observe currents
#ifdef USE_DASHBOARD
//    SmartDashboard::Log(topRoller.GetOutputCurrent(), "Top Current");
//    SmartDashboard::Log(bottomRoller.GetOutputCurrent(), "Bottom Current");
#endif

    topRoller.Set(dutyCycleSucking);
    bottomRoller.Set(dutyCycleSucking);
}

void Roller::RollOutward()
{
    topRoller.Set(dutyCycleSpittingTop);
    bottomRoller.Set(dutyCycleSpittingBottom);
}

void Roller::Stop()
{
    topRoller.Set(0.0);
    bottomRoller.Set(0.0);
}

void Roller::RollOpposite(bool rotateUpward)
{
    // set duty cycles based on rotation direction
    if(rotateUpward)
    {
        topRoller.Set(dutyCycleRotatingIn);
        bottomRoller.Set(dutyCycleRotatingOut);
    }
    else
    {
        topRoller.Set(dutyCycleRotatingOut);
        bottomRoller.Set(dutyCycleRotatingIn);
    }
}

void Roller::Output()
{
    // abort overrides everything
    if(action.master.abort)
        // stop moving rollers
        action.roller.state = action.roller.STOPPED;

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
    case ROTATING:
        RollOpposite(action.roller.rotateUpward);
        break;
    }
}

void Roller::Configure()
{
    Config& config = Config::GetInstance();

    // default values empirically determined on 3/11/11 in room 612 -KV
    dutyCycleSucking = config.Get<float>(prefix + "dutyCycleSucking", -1.0);

    // independent so that the ringer may be rotated when it is ejected
    dutyCycleSpittingTop = config.Get<float>(prefix + "dutyCycleSpittingTop", 1.0);
    dutyCycleSpittingBottom = config.Get<float>(prefix + "dutyCycleSpittingBottom", 0.6);

    // duty cycle for roller rotating inward is higher so that the ringer stays
    // inside the grabber (no tendency to move out)
    dutyCycleRotatingOut = config.Get<float>(prefix + "dutyCycleRotatingOut", -0.9);
    dutyCycleRotatingIn = config.Get<float>(prefix + "dutyCycleRotatingIn", 1.0);
}
