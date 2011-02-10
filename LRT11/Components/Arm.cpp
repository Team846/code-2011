#include "Arm.h"
#include "..\Config\RobotConfig.h"

Arm::Arm()
    : config(Config::GetInstance())
    , prefix("Arm.")
    , armEsc(RobotConfig::CAN_ARM, 1, prefix)
{
    armEsc.ConfigSoftPositionLimits(0, 10);
}

Arm::~Arm()
{

}

void Arm::Output()
{
    string key = prefix;
    float armSetPoint;

    if(action.arm.usePreset)
    {
        switch(action.arm.position)
        {
        case kLow:
            key += "low";
            break;
        case kHigh:
            key += "high";
            break;
        }

        armSetPoint = config.Get<float>(key);
    }
    else
        armSetPoint = action.arm.customSetpoint;

    armEsc.Set(armSetPoint);

    SmartDashboard::Log(armSetPoint, "Arm Set Point");
    SmartDashboard::Log(armEsc.GetPosition(), "Arm Position");
}
