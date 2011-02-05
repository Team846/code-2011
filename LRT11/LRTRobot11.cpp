#include "LRTRobot11.h"

LRTRobot11::LRTRobot11()
    : brain()
    , drive()
    , encoderData()
{
	
}

LRTRobot11::~LRTRobot11()
{

}

void LRTRobot11::RobotInit()
{
    Config& config = Config::GetInstance();
    config.ConfigureAll();
    config.Save();

    SmartDashboard::Log((Util::ToString<int>(config.Get<int>("BuildNumber")) + "-" +
            Util::ToString<int>(config.Get<int>("RunNumber"))).c_str(), "Build/Run");
}

void LRTRobot11::MainLoop()
{
    GameState state = DetermineState();
    brain.Process(state);

    if(!IsDisabled())
    {
        {
            ProfiledSection("Drive Train");
            drive.Output();
        }
        
        {
        	ProfiledSection("Encoder Data Collection");
        	encoderData.Output();
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
