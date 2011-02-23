#include "Brain.h"
#include "..\ActionData.h"
#include "Common.h"
#include "Disabled.h"
#include "Auton.h"
#include "Teleop.h"
#include "TeleopDriveTrain.h"
#include "TeleopShifter.h"
#include "TeleopLift.h"
#include "TeleopArm.h"
#include "TeleopRoller.h"

Brain::Brain()
    : config(Config::GetInstance())
    , console(Console::GetInstance())
    , lcd(LCD::GetInstance())
    , action(ActionData::GetInstance())
    , inputs(ProcessedInputs::GetInstance())
    , gameTimer()
    , leftEncoder(DriveEncoders::GetInstance().GetLeftEncoder())
    , rightEncoder(DriveEncoders::GetInstance().GetRightEncoder())
    , isFinale(false)
{

}

Brain::~Brain()
{

}

void Brain::Process(GameState gameState)
{
    if(previousState != gameState)
    {
        if(previousState == AUTONOMOUS && gameState == TELEOPERATED)
            gameTimer.Start();
    }

    Common();

    switch(gameState)
    {
    case DISABLED:
        Disabled();
        break;

    case TELEOPERATED:
        Teleop();
        break;

    case AUTONOMOUS:
        Auton();
        break;
    }

    // match is over
    if(gameTimer.Get() > 120)
        gameTimer.Stop();

    {
        ProfiledSection ps("Dashboard Logging");
        UpdateDashboardValues(gameState);
    }

    previousState = gameState;
}

void Brain::UpdateDashboardValues(GameState gameState)
{
    SmartDashboard::Log((int)(action.driveTrain.rawForward * 100), "Raw Forward (F)");
    SmartDashboard::Log(200 + (int)(action.driveTrain.rawForward * 100), "Raw Forward (B)");

    SmartDashboard::Log((int)(action.driveTrain.rawTurn * 100), "Raw Turn (F)");
    SmartDashboard::Log(200 + (int)(action.driveTrain.rawTurn * 100), "Raw Turn (B)");

    SmartDashboard::Log(action.driveTrain.brakeLeft, "BL: ");
    SmartDashboard::Log(action.driveTrain.brakeRight, "BR: ");

    SmartDashboard::Log(leftEncoder.Get(), "Left Ticks");
    SmartDashboard::Log(rightEncoder.Get(), "Right Ticks");

    SmartDashboard::Log(leftEncoder.GetRate(), "Left Rate");
    SmartDashboard::Log(rightEncoder.GetRate(), "Right Rate");

    if(previousState != DISABLED && gameState == DISABLED)
    {
        // log build and run number
        SmartDashboard::Log((Util::ToString<int>(config.Get<int>("BuildNumber")) + "-" +
                Util::ToString<int>(config.Get<int>("RunNumber"))).c_str(), "Build/Run");
    }

    SmartDashboard::Log(gameTimer.Get(), "Game Timer");

    // can only deploy minibot with 10 seconds remaining
    if(gameTimer.Get() > 110)
    {
        isFinale = true;
        SmartDashboard::Log(true, "Can Deploy Minibot?");
    }
    else
        SmartDashboard::Log(false, "Can Deploy Minibot?");
}
