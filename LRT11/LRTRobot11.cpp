#include "LRTRobot11.h"

LRTRobot11::LRTRobot11()
    : brain()
    , controller(CANBusController::GetInstance())
    , drive()
//    , positionDrive()
//    , encoderData()
    , shifter()
    , config(Config::GetInstance())
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

    SmartDashboard::Log(build, "Build/Run");
    AsynchronousPrinter::Printf(build);
}

void LRTRobot11::MainLoop()
{
    GameState gameState = DetermineState();
    brain.Process(gameState);

    // components to output regardless of state
    {
        ProfiledSection("Configuration Buttons");
        config.Output();
    }

    if(prevState != gameState)
        controller.ResetCache();

    if(gameState != DISABLED)
    {
        // components to output only when enabled
        {
            ProfiledSection("Drive Train");
            drive.Output();
        }

//        {
//            ProfiledSection("Position Drive");
//            positionDrive.Output();
//        }

        // {
        //    ProfiledSection("Encoder Data Collection");
        //    encoderData.Output();
        // }

        {
            ProfiledSection("Servo Shifting");
            shifter.Output();
        }

        // To add another component output:
        //
        // {
        //      ProfiledSection("Descriptive Phrase");
        //      component.Output();
        // }
    }

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
