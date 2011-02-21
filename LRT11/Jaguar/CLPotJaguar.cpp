#include "CLPotJaguar.h"

CLPotJaguar::CLPotJaguar(UINT8 channel, int potTurns, string configPrefix)
    : ProxiedCANJaguar(channel)
    , prefix(configPrefix)
    , turns(potTurns)
{
    Configure();
    EnableControl();
}

CLPotJaguar::~CLPotJaguar()
{
    // nothing
}

void CLPotJaguar::Configure()
{
    Config& config = Config::GetInstance();

    SetControlMode(CANJaguar::kPosition);
    SetPositionReference(CANJaguar::kPosRef_Potentiometer);

    SetPID(config.Get<double>(prefix + "pGain", 100), config.Get<double>(prefix + "iGain", 0),
            config.Get<double>(prefix + "dGain", 0));

    ConfigSoftPositionLimits(config.Get<double>(prefix + "forwardLimit", 0),
            config.Get<double>(prefix + "reverseLimit", turns));
    SetPotentiometerTurns(turns);
}
