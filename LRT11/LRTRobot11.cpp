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
//    , shifter()
    , lift()
    , arm()
    , minibotDeployer()
    , roller()
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

    // get rid of bogus reading 3/12/11 -KV
    LRTDigitalOutput::SetDelay(50 * 10); // 5 us; tested in room 612 on 3/12/11 -KV
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

//    if(prevState != gameState)
//        controller.ResetCache();

    if(gameState != DISABLED)
    {
//    LRTDigitalOutput::SetDelay((int)(50 * 100 * Util::Rescale<float>(ds.GetAnalogIn(1), 0, 5, 0, 1)));

        // components to output only when enabled

        {
            ProfiledSection ps("Drive Train");
            drive.Output();
        }

        {
            ProfiledSection ps("Lift");
            lift.Output();
        }

        // Tested successfully 3/10/11 [KV] [BA]
        {
            ProfiledSection ps("Arm");
            arm.Output();
        }

        {
            ProfiledSection ps("MinibotDeployer");
            minibotDeployer.Output();
        }

//        {
//            ProfiledSection ps("Position Drive");
//            positionDrive.Output();
//        }

//        {
//            ProfiledSection ps("Encoder Data Collection");
//            encoderData.Output();
//        }

//        {
//            ProfiledSection ps("Servo Shifting");
//            shifter.Output();
//        }

//        {
//            ProfiledSection ps("Roller");
//            roller.Output();
//        }


        // To add another component output:
        //
        // {
        //      ProfiledSection ps("Descriptive Phrase");
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
