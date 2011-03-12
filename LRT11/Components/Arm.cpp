#include "Arm.h"
#include "..\Config\RobotConfig.h"

Arm::Arm()
    : config(Config::GetInstance())
    , prefix("Arm.")
    , armEsc(RobotConfig::CAN_ARM)
#ifdef VIRTUAL
    // arm is ~29 inches
    // speed: 29 in * 1 ft / 12 in * 1.3 rps * 2 pi rad / rev = ~19.7 ft/s
    // ft / turn: 29 in * 1 ft / 12 in * 2 pi rad / rev = ~15.2 ft
    , armPot(RobotConfig::POT_ARM, 1, 15.2, 19.7)
#else
    , armPot(RobotConfig::POT_ARM)
#endif
    , cycleCount(0)
    , presetMode(true)
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

    timeoutCycles = (int)(config.Get<int>(prefix + "timeoutMs", 1500) * 1.0 / 1000.0 * 50.0 / 1.0);
}

void Arm::Output()
{
    static enum
    {
        IDLE,
        MANUAL,
        PRESET
    } state = IDLE;

    if(action.arm.givenCommand)
    {
        if(action.arm.manualMode)
        {
            presetMode = false;
            state = MANUAL;
        }
        else
        {
            cycleCount = timeoutCycles;
            presetMode = true;

            state = PRESET;
        }

        action.arm.givenCommand = false; // command has been processed
        action.arm.manualMode = false;
    }

    float potValue = armPot.GetAverageValue();
    SmartDashboard::Log(potValue, "Arm Pot Value");
//    if(state != IDLE)
//      potValue = liftEsc.GetPosition();
//        potValue = armPot.GetPosition();

    switch(state)
    {
    case IDLE:
        armEsc.Set(0.0);

        if(!presetMode)
            // exited from manual mode; done with maneuver
            action.arm.doneState = action.arm.SUCCESS;
        break;

    case MANUAL:
        action.arm.doneState = action.arm.STALE; // not done yet

        if(action.arm.manualUp)
        {
            if(potValue < maxPosition)
                armEsc.Set(powerUp);
            else
                armEsc.Set(0.0);
        }
        else
        {
            if(potValue > minPosition)
                armEsc.Set(powerDown);
            else
                armEsc.Set(0.0);
        }

        state = IDLE;
        break;

    case PRESET:
        action.lift.doneState = action.lift.STALE; // not done yet

        if(action.arm.presetTop)
        {
            if(potValue < maxPosition)
                armEsc.Set(powerUp);
            else
            {
                action.lift.doneState = action.lift.SUCCESS;
                cycleCount = 1; // will get decremented to 0
            }
        }
        else
        {
            if(potValue > minPosition)
                armEsc.Set(powerDown);
            else
            {
                action.lift.doneState = action.lift.SUCCESS;
                cycleCount = 1; // will get decremented to 0
            }
        }

        cycleCount--;

        if(cycleCount == 0)
        {
            if(action.lift.doneState != action.lift.SUCCESS)
                action.lift.doneState = action.lift.FAILURE;
            state = IDLE;
        }

        break;
    }
}

/*
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
        action.arm.done = false;
    }

    if(action.arm.manualUp || action.arm.presetTop)
    {
        if(potValue < maxPosition)
            armEsc.Set(powerUp);
        else if(action.arm.presetTop)
        {
            cycleCount = 1; // will get decremented to 0
            action.arm.done = true;
        }

//        if(action.arm.manualUp)
        // only run as long as button is down
    }
    else if(action.arm.manualDown || action.arm.presetBottom)
    {
        if(potValue > minPosition)
            armEsc.Set(powerDown);
        else if(action.arm.presetBottom)
        {
            cycleCount = 1; // will get decremented to 0
            action.arm.done = true;
        }

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

//    string key = prefix;
//    float armSetPoint = config.Get<float>(key + "top");
//
//    float sign;
//    switch(action.arm.position)
//    {
//    case BOTTOM:
//        key += "bottomRelative";
//        sign = -1;
//        break;
//    case TOP:
//        // no relative position
//        sign = 1;
//        break;
//    }
//
//    if(action.arm.position != action.arm.TOP)
//        armSetPoint += config.Get<int>(key);
//
//    input += Util::Sign<float>(armSetPoint - armPot.GetAverageValue()) * hysteresis;
//    armEsc.Set(input);
//
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
*/
