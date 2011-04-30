#include "Roller.h"
#include "..\Config\RobotConfig.h"

Roller::Roller()
    : topRoller(RobotConfig::CAN_ROLLER_TOP) // change port numbers later
    , bottomRoller(RobotConfig::CAN_ROLLER_BOTTOM)
    , prefix("Roller.")
    , ignoreCycles(25)
    , detected(false)
{
    topRoller.ConfigNeutralMode(LRTCANJaguar::kNeutralMode_Coast);
    bottomRoller.ConfigNeutralMode(LRTCANJaguar::kNeutralMode_Coast);

}

Roller::~Roller()
{

}

void Roller::RollInward()
{
//    topRoller.ShouldCollectCurrent(true);
//    bottomRoller.ShouldCollectCurrent(true);

//    float topCurrent, bottomCurrent;
//
//    {
//        ProfiledSection ps("Get Roller Currents");
//        topCurrent = topRoller.GetCurrent();
//        bottomCurrent = bottomRoller.GetCurrent();
//    }

//    static int cycleCount = 0;
//    if(++cycleCount % 10 == 0)
//    {
//        AsynchronousPrinter::Printf("I: %d, T: %.2f\n", ignoreCycles,
//                topCurrent + bottomCurrent);
//        fflush(stdout);
//    }

//    if(--ignoreCycles <= 0 && topCurrent + bottomCurrent > 15)
//        detected = true;

    // observe currents
#ifdef USE_DASHBOARD
//    SmartDashboard::Log(topRoller.GetOutputCurrent(), "Top Current");
//    SmartDashboard::Log(bottomRoller.GetOutputCurrent(), "Bottom Current");
#endif

//    if(detected)
//    {
//        topRoller.Set(0);
//        bottomRoller.Set(0);
//    }
//    else
//    {
    topRoller.Set(Util::Sign<float>(dutyCycleSucking) * Util::Min<float>(action.roller.maxSuckPower,
            Util::Abs<float>(dutyCycleSucking)));
    bottomRoller.Set(Util::Sign<float>(dutyCycleSucking) * Util::Min<float>(action.roller.maxSuckPower,
            Util::Abs<float>(dutyCycleSucking)));
//    }
}

void Roller::RollOutward()
{
    topRoller.Set(dutyCycleSpittingTop);
    bottomRoller.Set(dutyCycleSpittingBottom);
}

void Roller::Stop()
{
    topRoller.Set(0.0);
    bottomRoller.Set(0.0);
}

void Roller::RollOpposite(bool rotateUpward)
{
//    static int cycleCount = 0;

    // set duty cycles based on rotation direction
    if(rotateUpward)
    {
        topRoller.Set(dutyCycleRotatingIn);
        bottomRoller.Set(dutyCycleRotatingOut);
    }
    // pulse rotate and suck when rotating downward
    else
    {
        topRoller.Set(dutyCycleRotatingOut);
        bottomRoller.Set(dutyCycleRotatingIn);
    }
//    else if(++cycleCount < 20)
//    {
//        topRoller.Set(dutyCycleSucking);
//        bottomRoller.Set(dutyCycleSucking);
//    }
//    else
//        cycleCount = 0;
}

void Roller::Output()
{
    // abort overrides everything
    if(action.master.abort)
        // stop moving rollers
        action.roller.state = action.roller.STOPPED;

    if(action.roller.state != action.roller.SUCKING)
    {
        ignoreCycles = 25;
        detected = false;

        topRoller.ShouldCollectCurrent(false);
        bottomRoller.ShouldCollectCurrent(false);
    }

    switch(action.roller.state)
    {
    case STOPPED:
        Stop();
        break;
    case SUCKING:
        RollInward();
        break;
    case SPITTING:
        RollOutward();
        break;
    case ROTATING:
        RollOpposite(action.roller.rotateUpward);
        break;
    }
}

void Roller::Configure()
{
    Config& config = Config::GetInstance();

    // default values empirically determined on 3/11/11 in room 612 -KV
    dutyCycleSucking = config.Get<float>(prefix + "dutyCycleSucking", -1.0);

    // independent so that the ringer may be rotated when it is ejected
    dutyCycleSpittingTop = config.Get<float>(prefix + "dutyCycleSpittingTop", 1.0);
    dutyCycleSpittingBottom = config.Get<float>(prefix + "dutyCycleSpittingBottom", 0.6);

    // duty cycle for roller rotating inward is higher so that the ringer stays
    // inside the grabber (no tendency to move out)
    dutyCycleRotatingOut = config.Get<float>(prefix + "dutyCycleRotatingOut", -0.9);
    dutyCycleRotatingIn = config.Get<float>(prefix + "dutyCycleRotatingIn", 1.0);
}
