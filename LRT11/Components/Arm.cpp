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
        armSetPoint = config.Get<float>(key + "top");

        switch(action.arm.position)
        {
        case BOTTOM:
            key += "bottomRelative";
            break;
        case TOP:
            break; // no relative position
        }

        if(action.arm.position != action.arm.TOP)
            armSetPoint += config.Get<float>(key);
    }
    else
        armSetPoint = action.arm.customSetpoint;

    SmartDashboard::Log(armSetPoint, "Arm Set Point");
    SmartDashboard::Log(armEsc.GetPosition(), "Arm Position");
}
