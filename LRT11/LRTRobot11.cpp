#include "LRTRobot11.h"

LRTRobot11::LRTRobot11()
    : brain()
    , drive()
    , encoderData()
    , shifter()
    , config(Config::GetInstance())
{

}

LRTRobot11::~LRTRobot11()
{

}

void LRTRobot11::RobotInit()
{
    config.ConfigureAll();
    config.Save();

    SmartDashboard::Log((Util::ToString<int>(config.Get<int>("BuildNumber")) + "-" +
            Util::ToString<int>(config.Get<int>("RunNumber"))).c_str(), "Build/Run");
}

void LRTRobot11::MainLoop()
{
    GameState state = DetermineState();
    brain.Process(state);

    // components to output regardless of state
    {
        ProfiledSection("Configuration Buttons");
        config.Output();
    }

    if(!IsDisabled())
    {
        // components to output only when enabled
        {
            ProfiledSection("Drive Train");
            drive.Output();
        }

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
