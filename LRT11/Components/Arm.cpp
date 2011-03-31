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
    , pulseCount(0)
{
    armEsc.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
}

Arm::~Arm()
{

}

void Arm::Configure()
{
    minPosition = config.Get<float>(prefix + "minPosition", 280);
    maxPosition = config.Get<float>(prefix + "maxPosition", 530);

    powerUp = config.Get<float>(prefix + "powerUp", 0.30);
    powerRetainUp = config.Get<float>(prefix + "powerRetainUp", 0.10);
    powerDown = config.Get<float>(prefix + "powerDown", -0.15);

    timeoutCycles = (int)(config.Get<int>(prefix + "timeoutMs", 1500) * 1.0 / 1000.0 * 50.0 / 1.0);
}

void Arm::Output()
{
    float potValue = armPot.GetAverageValue();

#ifdef USE_DASHBOARD
    SmartDashboard::Log(potValue, "Arm Pot Value");
#endif


    if(oldState != action.arm.action)
        cycleCount = timeoutCycles;

    if(action.master.abort)
        action.arm.doneState = action.arm.ABORTED;
    else // do not allow normal processing in case of an abort
        switch(action.arm.action)
        {
        case PRESETTOP:
            if(potValue >= maxPosition)
            {
                action.arm.doneState = action.arm.SUCCESS;
                armEsc.Set(powerRetainUp);
                cycleCount = 1; // will get decremented to 0
            }
            else
            {
                action.arm.doneState = action.arm.STALE;
                armEsc.Set(powerUp);

                // make roller suck while moving up to keep
                // game piece in
                if(++pulseCount % 2 == 0)
                    action.roller.state = action.roller.SUCKING;
                else
                    action.roller.state = action.roller.STOPPED;
            }
            oldState = PRESETTOP;
            break;

        case PRESETBOTTOM:
            if(potValue < minPosition)
            {
                action.arm.doneState = action.arm.SUCCESS;
                armEsc.Set(0.0); // don't go below the min position
                cycleCount = 1; // will get decremented to 0
            }
            else
            {
                if(cycleCount <= 0)  //abort movement
                {
                    action.arm.action = action.arm.IDLE;
                    action.arm.doneState = action.arm.ABORTED;
                    armEsc.Set(0.0);
                }
                else
                {
                    armEsc.Set(powerDown);
                    action.arm.doneState = action.arm.STALE;
                }
            }
            oldState = PRESETBOTTOM;
            break;

        case MANUALUP:
            if(potValue < maxPosition)
                armEsc.Set(powerUp);
            else
                armEsc.Set(0.0);
            action.arm.doneState = action.arm.STALE;
            action.arm.action = action.arm.IDLE;
            oldState = MANUALUP;
            break;

        case MANUALDOWN:
            if(potValue > minPosition)
                armEsc.Set(powerDown);
            else
                armEsc.Set(0.0);
            action.arm.doneState = action.arm.STALE;
            action.arm.action = action.arm.IDLE;
            oldState = MANUALDOWN;
            break;

        case IDLE:
            action.arm.doneState = action.arm.SUCCESS;
            armEsc.Set(0.0);
            oldState = IDLE;
            break;
        }

    cycleCount--;
    if(action.arm.doneState == action.arm.SUCCESS)
        cycleCount = timeoutCycles;

    if(action.arm.doneState == action.arm.ABORTED)
    {
        armEsc.Set(0.0);
        action.arm.action = action.arm.IDLE;
        action.arm.doneState = action.arm.ABORTED;
    }
}
////////////////////////////////////////////////
//  static enum
//    {
//        IDLE,
//        ABORT,
//        MANUAL,
//        PRESET,
//        MAINTAINING
//    } state = IDLE;
//
//    if(action.arm.givenCommand)
//    {
//        if(action.arm.manualMode)
//        {
//            presetMode = false;
//            state = MANUAL;
//            state = MANUAL;
//        }
//        else
//        {
//            cycleCount = timeoutCycles;
//            presetMode = true;
//
//            state = PRESET;
//        }
//
//        action.arm.givenCommand = false; // command has been processed
//        action.arm.manualMode = false;
//    }
//
//    float potValue = armPot.GetAverageValue();
//#ifdef USE_DASHBOARD
//    SmartDashboard::Log(potValue, "Arm Pot Value");
//#endif
//
//    // abort overrides everything
//    if(action.master.abort)
//        state = ABORT;
//
//    switch(state)
//    {
//    case IDLE:
//        armEsc.Set(0.0);
//
//        if(!presetMode)
//            // exited from manual mode; done with maneuver
//            action.arm.doneState = action.arm.SUCCESS;
//        else
//        {
//            AsynchronousPrinter::Printf("Maintaining arm position.\n");
//
//            // arm has drifted from preset value
//            if(action.arm.doneState == action.arm.SUCCESS &&
//                    ((potValue < maxPosition && action.arm.presetTop) ||
//                            (potValue > minPosition && !action.arm.presetTop)))
//            {
//                AsynchronousPrinter::Printf("Arm position inside if.\n");
//
//                // reset timer
//                cycleCount = timeoutCycles;
//                state = PRESET; // switch back to preset mode
//            }
//        }
//        break;
//
//    case ABORT:
//        armEsc.Set(0.0);
//        action.arm.doneState = action.arm.ABORTED;
//        break;
//
//    case MANUAL:
//        action.arm.doneState = action.arm.STALE; // not done yet
//
//        if(action.arm.manualUp)
//        {
//            if(potValue < maxPosition)
//                armEsc.Set(powerUp);
//            else
//                armEsc.Set(0.0);
//        }
//        else
//        {
//            if(potValue > minPosition)
//                armEsc.Set(powerDown);
//            else
//                armEsc.Set(0.0);
//        }
//
//        state = IDLE;
//        break;
//
//    case PRESET:
//        action.arm.doneState = action.arm.STALE; // not done yet
//
//        if(action.arm.presetTop)
//        {
//            if(potValue < maxPosition)
//            {
//                armEsc.Set(powerUp);
//
//                // make roller suck while moving up to keep
//                // game piece in
//                if(++pulseCount % 2 == 0)
//                    action.roller.state = action.roller.SUCKING;
//                else
//                    action.roller.state = action.roller.STOPPED;
//            }
//            else
//            {
//                action.arm.doneState = action.arm.SUCCESS;
//                armEsc.Set(0.0); // don't go above the max position
//                cycleCount = 1; // will get decremented to 0
//            }
//        }
//        else
//        {
//            if(potValue > minPosition)
//                armEsc.Set(powerDown);
//            else
//            {
//                action.arm.doneState = action.arm.SUCCESS;
//                armEsc.Set(0.0); // don't go below the min position
//                cycleCount = 1; // will get decremented to 0
//            }
//        }
//
//        cycleCount--;
//
//        if(cycleCount == 0)
//        {
//            if(action.arm.doneState != action.arm.SUCCESS)
//                action.arm.doneState = action.arm.FAILURE;
//
//            if(action.arm.presetTop)
//                state = MAINTAINING;
//            else
//                state = IDLE;
//        }
//
//        break;
//
//    case MAINTAINING:
//        action.arm.doneState = action.arm.SUCCESS;
//
//        if(potValue < maxPosition - ARM_UP_THRESHOLD)
//            armEsc.Set(powerUp);
//        else
//            armEsc.Set(powerRetainUp);
//
//        break;
//    }
//}


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
