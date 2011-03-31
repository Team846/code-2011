#include "LRTRobot11.h"

LRTRobot11::LRTRobot11()
    : brain()
#ifdef VIRTUAL
    , controller(VirtualCANBusController::GetInstance())
#else
//    , controller(CANBusController::GetInstance())
#endif
    , drive()
//    , positionDrive()
//    , encoderData()
    , shifter()
    , lift()
    , arm()
    , roller()
    , minibotDeployer()
    , config(Config::GetInstance())
    , ds(*DriverStation::GetInstance())
    , switchLED(6)
    , prevState(DISABLED)
{

}

LRTRobot11::~LRTRobot11()
{

}

void LRTRobot11::RobotInit()
{
    config.ConfigureAll();
    config.Save();

    const char* build = (Util::ToString<int>(config.Get<int>("BuildNumber")) + "-" +
            Util::ToString<int>(config.Get<int>("RunNumber"))).c_str();

#ifdef USE_DASHBOARD
    SmartDashboard::Log(build, "Build/Run");
#endif
    AsynchronousPrinter::Printf(build);
}

void LRTRobot11::MainLoop()
{
    GameState gameState = DetermineState();

    {
        ProfiledSection ps("Brain Processing");
        brain.Process(gameState);
    }

    // components to output regardless of state
    {
        ProfiledSection ps("Configuration Buttons");
        config.Output();
    }

#ifndef VIRTUAL
    ProxiedCANJaguar::SetGameState(gameState);
#endif

//    if(prevState != gameState)
//        controller.ResetCache();

//    if(gameState != DISABLED)
//    {
    // components to output only when enabled

    if(ds.GetDigitalIn(1))
    {
        ProfiledSection ps("Drive Train");
        drive.Output();
    }

    if(ds.GetDigitalIn(2))
    {
//            ProfiledSection ps("Position Drive");
//            positionDrive.Output();
    }

    if(ds.GetDigitalIn(3))
    {
        ProfiledSection ps("Lift");
        lift.Output();
    }

    if(ds.GetDigitalIn(4))
    {
        ProfiledSection ps("Arm");
        arm.Output();
    }

//        {
//            ProfiledSection ps("Encoder Data Collection");
//            encoderData.Output();
//        }

    if(ds.GetDigitalIn(5))
    {
        ProfiledSection ps("Servo Shifting");
        shifter.Output();
    }

    if(ds.GetDigitalIn(6))
    {
        ProfiledSection ps("Roller");
        roller.Output();
    }

    if(ds.GetDigitalIn(7))
    {
        ProfiledSection ps("Minibot Deployment");
        minibotDeployer.Output();
    }

    // To add another component output:
    //
    // {
    //      ProfiledSection ps("Descriptive Phrase");
    //      component.Output();
    // }
//    }

    prevState = gameState;
}

GameState LRTRobot11::DetermineState()
{
    GameState state = TELEOPERATED;

    if(IsDisabled())
        state = DISABLED;
    else if(IsAutonomous())
        state = AUTONOMOUS;

    return state;
}

START_ROBOT_CLASS(LRTRobot11);
