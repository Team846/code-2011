#include "CLPotJaguar.h"

CLPotJaguar::CLPotJaguar(UINT8 channel, int turns, string configPrefix, double defaultP, double defaultI, double defaultD)
    : ProxiedCANJaguar(channel)
    , config(Config::GetInstance())
    , prefix(configPrefix)
{
    // configuration to closed loop position control mode
    SetControlMode(CANJaguar::kPosition);
    SetPositionReference(CANJaguar::kPosRef_Potentiometer);
    SetPotentiometerTurns(turns);

    // pid configuration
    SetPID(config.Get<double>(prefix + "pGain", defaultP), config.Get<double>(prefix + "iGain", defaultI),
            config.Get<double>(prefix + "dGain", defaultD));

    EnableControl();
}

CLPotJaguar::~CLPotJaguar()
{
    // nothing
}

void CLPotJaguar::LoadSoftPositionLimits(double defaultForwardLimit, double defaultReverseLimit)
{
    double forwardLimit = config.Get<double>(prefix + "forwardLimit", defaultForwardLimit);
    double reverseLimit = config.Get<double>(prefix + "reverseLimit", defaultReverseLimit);
    ConfigSoftPositionLimits(forwardLimit, reverseLimit);
}
