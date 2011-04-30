#include "Lift.h"
#include "..\Config\RobotConfig.h"

Lift::Lift()
    : config(Config::GetInstance())
    , prefix("Lift.")
    , liftEsc(RobotConfig::CAN_LIFT)
#ifdef VIRTUAL
    , liftPot(RobotConfig::CAN_LIFT, 10, 1.0, 6.5)
#endif
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
//    liftEsc.SetControlMode(CANJaguar::kPosition);
    liftEsc.ChangeControlMode(LRTCANJaguar::kPosition);
#ifdef VIRTUAL
    liftEsc.SetPositionReference(&liftPot);
#else
    liftEsc.SetPositionReference(LRTCANJaguar::kPosRef_Potentiometer);
#endif

    liftEsc.SetPID(config.Get<double>(prefix + "pGain", 100), config.Get<double>(prefix + "iGain", 0),
            config.Get<double>(prefix + "dGain", 0));

//    liftEsc.ConfigSoftPositionLimits(config.Get<double>(prefix + "forwardLimit", 0),
//            config.Get<double>(prefix + "reverseLimit", 10));
//    liftEsc.SetPotentiometerTurns(10);
    liftEsc.ConfigPotentiometerTurns(10);

    Config& config = Config::GetInstance();

    // convert from ms into cycles
    timeoutCycles = (int)(config.Get<int>(prefix + "timeoutMs", 1500) * 1.0 / 1000.0 * 50.0 / 1.0);

    // bottom of low row is the lowest position
    minPosition = config.Get<float>(prefix + "lowRowReference", 1.17)
            + config.Get<float>(prefix + "lowRowLowPegRelative", 0.7);

    // bottom of high row + high peg relative is the highest position
    maxPosition = config.Get<float>(prefix + "highRowReference", 1.87)
            + config.Get<float>(prefix + "highPegRelative", 6.5);

    potDeadband = config.Get<float>(prefix + "deadband", 0.4);
}

void Lift::ConfigureManualMode()
{
//    liftEsc.SetControlMode(CANJaguar::kPercentVbus);
    liftEsc.ChangeControlMode(LRTCANJaguar::kPercentVbus);
    liftEsc.EnableControl();
}

void Lift::Output()
{
    static enum
    {
        IDLE,
        ABORT,
        MANUAL,
        PRESET,
        PULSING
    } state = IDLE;

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
#ifdef VIRTUAL
    potValue = liftPot.GetPotValue();
#else
    potValue = liftEsc.GetPotValue();
#endif

#ifdef USE_DASHBOARD
    SmartDashboard::Log(potValue, "Lift Pot Value");
#endif

    // abort overrides everything
    if(action.master.abort)
        state = ABORT;

    static int potCycleCount = 0;
    static bool shouldMoveArmToMiddle = false;

    switch(state)
    {
    case IDLE:
//        AsynchronousPrinter::Printf("Idle\n");
        liftEsc.DisableControl();
        liftEsc.ShouldCollectPotValue(false);

        if(++potCycleCount % 50 == 0)
            liftEsc.ShouldCollectPotValue(true);

        if(!positionMode)
        {
            // exited from manual mode; done with maneuver
            action.lift.doneState = action.lift.SUCCESS;
            liftEsc.Set(0.0);
        }
        else if(shouldMoveArmToMiddle)
            action.arm.state = action.arm.PRESET_MIDDLE;
        break;

    case ABORT:
//        AsynchronousPrinter::Printf("Abort\n");
        liftEsc.DisableControl();
        liftEsc.ShouldCollectPotValue(false);

        if(!positionMode)
            liftEsc.Set(0.0);

        action.lift.doneState = action.lift.ABORTED;
        break;

    case PULSING:
//        AsynchronousPrinter::Printf("Pulsing\n");
        liftEsc.ShouldCollectPotValue(true);
        if(positionMode)
        {
            // configure jaguar for voltage mode
            ConfigureManualMode();
            positionMode = false;
        }

        if(potValue >= minPosition)
        {
            liftEsc.ResetCache();
            liftEsc.Set(-0.1);
        }
        break;

    case MANUAL:
//        AsynchronousPrinter::Printf("Manual\n");
        liftEsc.ShouldCollectPotValue(true);
        action.lift.doneState = action.lift.IN_PROGRESS; // not done yet

        if((action.lift.power > 0 && potValue < maxPosition) ||
                (action.lift.power < 0 && potValue > minPosition))
        {
            liftEsc.ResetCache();
            liftEsc.Set(action.lift.power);
        }
        else
            // don't power past the minimum and maximum positions
            liftEsc.Set(0.0);

        state = IDLE;
        break;

    case PRESET:
//        AsynchronousPrinter::Printf("Preset\n");
        liftEsc.ShouldCollectPotValue(true);
        action.lift.doneState = action.lift.IN_PROGRESS; // not done yet
        string key = prefix;

        float setpoint = 0.0;
        if(action.lift.highColumn)
            key += "highColumn.";
//            setPoint = config.Get<float>(prefix + "highRowReference");
        else
            key += "lowColumn.";
//            setPoint = config.Get<float>(prefix + "lowRowReference");

        switch(action.lift.preset)
        {
        case STOWED:
            setpoint = 0; // no movement
            break;
        case LOW_PEG:
            key += "lowPeg";
            break;
        case MED_PEG:
            key += "mediumPeg";
            break;
        case HIGH_PEG:
            key += "highPeg";
            break;
        }

        if(action.lift.preset != action.lift.STOWED)
            setpoint = config.Get<float>(key); // relative to bottom

//        AsynchronousPrinter::Printf("Status: %.2f\n", Util::Abs<float>(potValue - setPoint));
        // update done flag
        if(Util::Abs<float>(potValue - setpoint) < potDeadband)
        {
//            AsynchronousPrinter::Printf("Updating done flag");
            action.lift.doneState = action.lift.SUCCESS;
//            cycleCount = 1; // will get decremented to 0

            if(action.lift.preset == action.lift.MED_PEG || action.lift.preset == action.lift.HIGH_PEG)
            {
//                AsynchronousPrinter::Printf("Lift success; moving arm to middle position\n");
                action.arm.state = action.arm.PRESET_MIDDLE;
                shouldMoveArmToMiddle = true;
            }
        }
        else
            // keep arm upright when the lift is moving
            action.arm.state = action.arm.PRESET_TOP;

        SmartDashboard::Log(setpoint, "Lift Set Point");
        liftEsc.Set(setpoint);
        cycleCount--;

        if(cycleCount == 0)
        {
            if(action.lift.doneState != action.lift.SUCCESS)
            {
                action.lift.doneState = action.lift.FAILURE;
                shouldMoveArmToMiddle = false;
            }

            if(action.lift.preset == action.lift.LOW_PEG &&
                    action.lift.doneState == action.lift.SUCCESS)
                state = PULSING;
            else
                state = IDLE;
        }

#ifdef USE_DASHBOARD
//        SmartDashboard::Log(setPoint, "Lift Set Point");
#endif
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
