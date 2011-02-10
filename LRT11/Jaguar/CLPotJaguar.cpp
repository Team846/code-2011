#include "CLPositionJaguar.h"

CLPotJaguar::CLPotJaguar(UINT8 channel, int turns, string prefix, double defaultP, double defaultI, double defaultD)
    : ProxiedCANJaguar(channel)
    , config(Config::GetInstance())
    , this->prefix(prefix)
{
    // configuration to closed loop position control mode
    SetControlMode(CANJaguar::kPosition);
    SetPositionReference(CANJaguar::kPosRef_Potentiometer);
    SetPotentiometerTurns(turns);

    // pid configuration
    SetPID(config.Get<int>(prefix + "pGain", defaultP), config.Get<int>(prefix + "iGain", defaultI),
            config.Get<int>(prefix + "dGain", defaultD));

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
}