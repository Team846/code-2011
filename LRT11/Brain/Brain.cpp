#include "Brain.h"
#include "Common.h"
#include "Disabled.h"
#include "Auton.h"
#include "Teleop.h"
#include "..\ActionData.h"

Brain::Brain()
    : config(Config::GetInstance())
    , console(Console::GetInstance())
    , lcd(LCD::GetInstance())
    , action(ActionData::GetInstance())
    , inputs(ProcessedInputs::GetInstance())
    , isCompetition(false)
    , leftEncoder(DriveEncoders::GetInstance().GetLeftEncoder())
    , rightEncoder(DriveEncoders::GetInstance().GetRightEncoder())
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
            isCompetition = true;
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

    UpdateDashboardValues( gameState );
    previousState = gameState;
}

void Brain::UpdateDashboardValues( GameState gameState)
{
	SmartDashboard::Log( (int) ( action.driveTrain.rawForward * 100 ), "Raw Forward (F)" );
	SmartDashboard::Log( 100 - (int) ( action.driveTrain.rawForward * 100 ), "Raw Forward (B)" );
	
	SmartDashboard::Log( (int) ( action.driveTrain.rawTurn * 100 ), "Raw Turn (F)" );
	SmartDashboard::Log( 100 - (int) ( action.driveTrain.rawTurn * 100 ), "Raw Turn (B)" );
	
	SmartDashboard::Log( action.driveTrain.brakeLeft, "BL: " );
	SmartDashboard::Log( action.driveTrain.brakeRight, "BR: " );
	
	SmartDashboard::Log( leftEncoder.Get(), "Left Ticks" );
	SmartDashboard::Log( rightEncoder.Get(), "Right Ticks" );
	
	SmartDashboard::Log( leftEncoder.GetRate(), "Left Rate" );
	SmartDashboard::Log( rightEncoder.GetRate(), "Right Rate" );
	
	if( previousState != DISABLED && gameState == DISABLED )
	{
		// log build and run number
		SmartDashboard::Log( ( Util::ToString<int>( config.Get<int>( "BuildNumber" ) ) + "-" + 
			Util::ToString<int>( config.Get<int>( "RunNumber" ) ) ).c_str(), "Build/Run" );
	}
}	
