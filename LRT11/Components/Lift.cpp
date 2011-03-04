#include "Lift.h"
#include "..\Config\RobotConfig.h"

Lift::Lift()
    : config(Config::GetInstance())
    , prefix("Lift.")
    , liftEsc(RobotConfig::CAN_LIFT)
    , timeoutMs(0)
    , cycleCount(0)
    , prevMode(PRESET)
    , potDeadband(0)
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
    timeoutMs = config.Get<int>(prefix + "timeoutMs", 1500);

    minPosition = config.Get<float>(prefix + "lowRowBottom");
    maxPosition = minPosition + config.Get<float>(prefix + "highPegRelative");

    potDeadband = config.Get<float>(prefix + "deadband", 0.3);
}

void Lift::ConfigureVoltageMode()
{
    liftEsc.SetControlMode(CANJaguar::kPercentVbus);
    liftEsc.EnableControl();
}

void Lift::Output()
{
    float potValue;

    {
        ProfiledSection ps("Lift Log Position");
        potValue = liftEsc.GetPosition();
        SmartDashboard::Log(potValue, "Lift Pot Value");
    }

    if(!action.lift.givenCommand && cycleCount == 0)
    {
        {
            ProfiledSection ps("Lift disable control");
            AsynchronousPrinter::Printf("Disabling control, yo\n");
            liftEsc.DisableControl();
        }
        return;
    }
    else if((cycleCount == 0 || action.lift.givenCommand) && !action.lift.manualMode)
    {
        action.lift.givenCommand = false;
        StartTimer();

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

        // update done status
        if(Util::Abs<float>(potValue - setPoint) < potDeadband)
            action.lift.done = true;
        else
            action.lift.done = false;

        liftEsc.Set(setPoint);
        cycleCount--;

        SmartDashboard::Log(setPoint, "Lift Set Point");
        prevMode = PRESET;
    }
}

void Lift::StartTimer()
{
    cycleCount = (int)(timeoutMs * 1.0 / 1000 * 50.0 / 1);
}
