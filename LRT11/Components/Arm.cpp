#include "Arm.h"
#include "..\Config\RobotConfig.h"

Arm::Arm()
    : config(Config::GetInstance())
    , prefix("Arm.")
    , armEsc(RobotConfig::CAN_ARM)
    , armPot(RobotConfig::POT_ARM)
    , cycleCount(0)
{
    armEsc.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
}

Arm::~Arm()
{

}

void Arm::Configure()
{
    minPosition = config.Get<float>(prefix + "minPosition", 280);
    maxPosition = config.Get<float>(prefix + "maxPosition", 530);

    powerUp = config.Get<float>(prefix + "powerUp", 0.30);
    powerDown = config.Get<float>(prefix + "powerDown", -0.15);

    timeoutMs = config.Get<int>(prefix + "timeoutMs", 1500);
}

void Arm::Output()
{
    float potValue = armPot.GetAverageValue();
    SmartDashboard::Log(potValue, "Arm Pot Value");

    if(!action.arm.givenCommand && cycleCount == 0)
    {
        armEsc.Set(0);
        return;
    }
    else if((cycleCount == 0 || action.arm.givenCommand) && (action.arm.presetTop || action.arm.presetBottom))
    {
        cycleCount = (int)(timeoutMs * 1.0 / 1000 * 50.0 / 1);
    }

    if(action.arm.manualUp || action.arm.presetTop)
    {
        if(potValue < maxPosition)
            armEsc.Set(powerUp);
        else if(action.arm.presetTop)
            cycleCount = 1; // will get decremented to 0

//        if(action.arm.manualUp)
        // only run as long as button is down
    }
    else if(action.arm.manualDown || action.arm.presetBottom)
    {
        if(potValue > minPosition)
            armEsc.Set(powerDown);
        else if(action.arm.presetBottom)
            cycleCount = 1; // will get decremented to 0

//        if(action.arm.manualDown)
        // only run as long as button is down
    }

    action.arm.givenCommand = false;
    if(action.arm.presetTop || action.arm.presetBottom)
    {
        cycleCount--;

        if(cycleCount == 0)
        {
            action.arm.presetTop = false;
            action.arm.presetBottom = false;
        }
    }

    /*
    string key = prefix;
    float armSetPoint = config.Get<float>(key + "top");

    float sign;
    switch(action.arm.position)
    {
    case BOTTOM:
        key += "bottomRelative";
        sign = -1;
        break;
    case TOP:
        // no relative position
        sign = 1;
        break;
    }

    if(action.arm.position != action.arm.TOP)
        armSetPoint += config.Get<int>(key);
    */



//    input += Util::Sign<float>(armSetPoint - armPot.GetAverageValue()) * hysteresis;
//    armEsc.Set(input);

//    string key = prefix;
//    float armSetPoint;
//
//    if(action.arm.usePreset)
//    {
//        armSetPoint = config.Get<float>(key + "top");
//
//        switch(action.arm.position)
//        {
//        case BOTTOM:
//            key += "bottomRelative";
//            break;
//        case TOP:
//            break; // no relative position
//        }
//
//        if(action.arm.position != action.arm.TOP)
//            armSetPoint += config.Get<float>(key);
//    }
//    else
//        armSetPoint = action.arm.customSetpoint;
//
//    SmartDashboard::Log(armSetPoint, "Arm Set Point");
//    SmartDashboard::Log(armEsc.GetPosition(), "Arm Position");
}
