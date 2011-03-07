#include "Lift.h"
#include "..\Config\RobotConfig.h"

Lift::Lift()
    : config(Config::GetInstance())
    , prefix("Lift.")
    , liftEsc(RobotConfig::CAN_LIFT)
    , liftPot(RobotConfig::CAN_LIFT, 10, 1.0, 6.5)
    , timeoutCycles(0)
    , cycleCount(0)
    , prevMode(PRESET)
    , potDeadband(0)
    , positionMode(true)
{

}

Lift::~Lift()
{

}

void Lift::Configure()
{
    liftEsc.SetControlMode(CANJaguar::kPosition);
//    liftEsc.ChangeControlMode(CANJaguar::kPosition);
    liftEsc.SetPositionReference(CANJaguar::kPosRef_Potentiometer);

    liftEsc.SetPID(config.Get<double>(prefix + "pGain", 100), config.Get<double>(prefix + "iGain", 0),
            config.Get<double>(prefix + "dGain", 0));

//    liftEsc.ConfigSoftPositionLimits(config.Get<double>(prefix + "forwardLimit", 0),
//            config.Get<double>(prefix + "reverseLimit", 10));
    liftEsc.SetPotentiometerTurns(10);
//    liftEsc.ConfigPotentiometerTurns(10);

    liftEsc.EnableControl();

    Config& config = Config::GetInstance();

    // convert from ms into cycles
    timeoutCycles = (int)(config.Get<int>(prefix + "timeoutMs", 1500) * 1.0 / 1000.0 * 50.0 / 1.0);

    minPosition = config.Get<float>(prefix + "lowRowBottom");
    maxPosition = minPosition + config.Get<float>(prefix + "highPegRelative");

    potDeadband = config.Get<float>(prefix + "deadband", 0.3);
}

void Lift::ConfigureManualMode()
{
    liftEsc.SetControlMode(CANJaguar::kPercentVbus);
    liftEsc.EnableControl();
}

void Lift::Output()
{
    static enum
    {
        IDLE,
        MANUAL,
        PRESET
    } state;

    if(action.lift.givenCommand)
    {
        if(action.lift.manualMode)
        {
            if(positionMode)
            {
                // configure jaguar for percent voltage mode
                ConfigureManualMode();
                positionMode = false;
            }

            state = MANUAL;
        }
        else
        {
            if(!positionMode)
            {
                // configure jaguar for position mode
                Configure();
                positionMode = true;
            }

            cycleCount = timeoutCycles;
            state = PRESET;
        }

        action.lift.givenCommand = false; // command has been processed
        liftEsc.EnableControl();
    }

    float potValue = 0.0;
    if(state != IDLE)
//      potValue = liftEsc.GetPosition();
        potValue = liftPot.GetPosition();

    switch(state)
    {
    case IDLE:
        liftEsc.DisableControl();

        if(!positionMode)
        {
            // just exited from manual mode; done with maneuver
            action.lift.doneState = action.lift.SUCCESS;
            liftEsc.Set(0.0);
        }
        break;

    case MANUAL:
        action.lift.doneState = action.lift.STALE; // not done yet

        if((action.lift.power > 0 && potValue < maxPosition) ||
                (action.lift.power < 0 && potValue > minPosition))
        {
            liftEsc.ResetCache();
            liftEsc.Set(action.lift.power);
        }

        state = IDLE;
        break;

    case PRESET:
        action.lift.doneState = action.lift.STALE; // not done yet
        string key = prefix;

        float setPoint;
        if(action.lift.highRow)
            setPoint = config.Get<float>(prefix + "highRowBottom");
        else
            setPoint = config.Get<float>(prefix + "lowRowBottom");

        switch(action.lift.position)
        {
        case STOWED:
            break; // no relative position
        case LOW_PEG:
            key += "lowPegRelative";
            break;
        case MED_PEG:
            key += "mediumPegRelative";
            break;
        case HIGH_PEG:
            key += "highPegRelative";
            break;
        }

        if(action.lift.position != action.lift.STOWED)
            setPoint += config.Get<float>(key); // relative to bottom

        // update done flag
        if(Util::Abs<float>(potValue - setPoint) < potDeadband)
            action.lift.doneState = action.lift.SUCCESS;

        liftEsc.Set(setPoint);
        cycleCount--;

        if(cycleCount == 0)
        {
            if(action.lift.doneState != action.lift.SUCCESS)
                action.lift.doneState = action.lift.FAILURE;
            state = IDLE;
        }

        SmartDashboard::Log(setPoint, "Lift Set Point");
        break;
    }
}

/*
void Lift::Output()
{
    float potValue;

    {
        ProfiledSection ps("Lift Log Position");
//        potValue = liftEsc.GetPosition();
        potValue = liftPot.GetPosition();
        SmartDashboard::Log(potValue, "Lift Pot Value");
    }

    if(!action.lift.givenCommand && cycleCount == 0)
    {
        {
            ProfiledSection ps("Lift disable control");
            liftEsc.DisableControl();
        }
        return;
    }
    else if((cycleCount == 0 || action.lift.givenCommand) && !action.lift.manualMode)
    {
        action.lift.givenCommand = false;
        StartTimer();

        // reset preset flags
        action.lift.done = false;
        liftEsc.EnableControl();
    }

    if(action.lift.manualMode)
    {
        liftEsc.EnableControl();

        if(prevMode == PRESET)
            ConfigureVoltageMode();

        if((action.lift.power > 0 && potValue < maxPosition) ||
                (action.lift.power < 0 && potValue > minPosition))
        {
            liftEsc.ResetCache();
            liftEsc.Set(action.lift.power);
        }

        action.lift.givenCommand = false;
        prevMode = MANUAL;
    }
    else
    {
        if(prevMode == MANUAL)
            Configure(); // position control mode

        string key = prefix;

        float setPoint;
        if(action.lift.highRow)
            setPoint = config.Get<float>(prefix + "highRowBottom");
        else
            setPoint = config.Get<float>(prefix + "lowRowBottom");

        switch(action.lift.position)
        {
        case STOWED:
            break; // no relative position
        case LOW_PEG:
            key += "lowPegRelative";
            break;
        case MED_PEG:
            key += "mediumPegRelative";
            break;
        case HIGH_PEG:
            key += "highPegRelative";
            break;
        }

        if(action.lift.position != action.lift.STOWED)
            setPoint += config.Get<float>(key); // relative to bottom

        // update done flag
        if(Util::Abs<float>(potValue - setPoint) < potDeadband)
            action.lift.done = true;

        liftEsc.Set(setPoint);
        cycleCount--;

        SmartDashboard::Log(setPoint, "Lift Set Point");
        prevMode = PRESET;
    }
}
*/
