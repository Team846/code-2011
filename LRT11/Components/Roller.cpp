#include "Roller.h"
#include "..\Config\RobotConfig.h"

Roller::Roller()
    : topRoller(RobotConfig::CAN_ROLLER_TOP) // change port numbers later
    , bottomRoller(RobotConfig::CAN_ROLLER_BOTTOM)
    , prefix("Roller.")
{
	
}

Roller::~Roller()
{

}

void Roller::RollInward()
{
    topRoller.Set(dutyCycleSucking);
    bottomRoller.Set(dutyCycleSucking);
}

void Roller::RollOutward()
{
    topRoller.Set(dutyCycleSpitting);
    bottomRoller.Set(dutyCycleSpitting);
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

    // 6V is sufficient for sucking (3/2/11)
    dutyCycleSucking = config.Get<float>(prefix + "dutyCycleSucking", 0.5);
    
    // 12V is ideal for spitting out the ringer at a range of about 3-15 inches (3/2/11)
    dutyCycleSpitting = config.Get<float>(prefix + "dutyCycleSpitting", -1.0);
    
    // 3V results in a good speed for rotating the ringer (3/2/11)
    dutyCycleRotatingOut = config.Get<float>(prefix + "dutyCycleRotatingOut", -0.25);
    
    // duty cycle for roller rotating inward is higher so that the ringer stays 
    // inside the grabber (no tendency to move out)
    dutyCycleRotatingIn = config.Get<float>(prefix + "dutyCycleRotatingIn", 0.30);
}
