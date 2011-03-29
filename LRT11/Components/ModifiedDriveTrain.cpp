#include "ModifiedDriveTrain.h"

ModifiedDriveTrain::ModifiedDriveTrain()
    : Component()
    , closedRateTrain()
    , closedLoopPositionTrain(closedRateTrain)
#ifdef LRT_ROBOT_2011
    , leftESC(RobotConfig::CAN_DRIVE_LEFT_A, RobotConfig::CAN_DRIVE_LEFT_B,
            DriveEncoders::GetInstance().GetLeftEncoder(), "left")
    , rightESC(RobotConfig::CAN_DRIVE_RIGHT_A, RobotConfig::CAN_DRIVE_RIGHT_B,
            DriveEncoders::GetInstance().GetRightEncoder(), "right")
#else
    // TODO fix initialization
    , leftESC(RobotConfig::CAN_DRIVE_LEFT, DriveEncoders::GetInstance().GetLeftEncoder(), "left")
    , rightESC(RobotConfig::CAN_DRIVE_RIGHT, DriveEncoders::GetInstance().GetRightEncoder(), "right")
#endif
{

}

ModifiedDriveTrain::~ModifiedDriveTrain()
{

}

//TODO add braking
void ModifiedDriveTrain::Output()
{
	DriveCommand drive;
	if (action.driveTrain.position.resetFwd)
	{
		action.driveTrain.position.resetFwd = false;
		closedLoopPositionTrain.position.ResetFwd();
	}
	if (action.driveTrain.position.resetTurn)
	{
		action.driveTrain.position.resetTurn = false;
		closedLoopPositionTrain.ResetTurn();
	}
	
	if (action.driveTrain.distance.givenCommand)
	{
		action.driveTrain.givenCommand = false;
	}
	
    // calculate left duty cycle, right duty cycle, left brake, and
    // right brake based off of joystick inputs
	switch (action.driveTrain.mode)
	{
		case action.driveTrain.SPEED:
			drive = closedRateTrain.Drive(action.driveTrain.speed.rawForward,
	            action.driveTrain.speed.rawTurn);
			break;
		
		case POSITION:
			drive = closedLoopPositionTrain.Drive( 
					action.driveTrain.position.distanceSetPoint
					, action.driveTrain.position.bearingSetPoint
					, action.driveTrain.position.maxFwdSpeed
					, action.driveTrain.position.maxTurnSpeed
					, action.driveTrain.position.stop);
			break;
			
		case DISTANCE:
			CLPositionCommand command = 
				closedLoopPositionTrain.DriveAtLeastDistance(action.driveTrain.distance.distanceDutyCycle);
			action.driveTrain.distance.done = command.isDone;
			drive = command.drive;
			break;
	} 
	
    // leftDC and rightDC are set to 0 if there is a need to brake;
    // see DitheredBrakeTrain's Drive method
    leftESC.Set(drive.leftCommand.dutyCycle);
    rightESC.Set(drive.rightCommand.dutyCycle);

    // leftBrakeDC and rightBrakeDC are 0 if a PWM duty cycle is being
    // sent; see DitheredBrakeTrain's Drive method

    // leftBrakeDC and rightBrakeDC must be converted from a percent to a
    // value in range [1,8]; 1 means no braking while 8 means max braking
    leftESC.SetBrake((int)(drive.leftCommand.brakingDutyCycle * 8));
    rightESC.SetBrake((int)(drive.rightCommand.brakingDutyCycle * 8));

    // apply brakes only has an effect if SetBrake is called with a
    // non-zero parameter
    leftESC.ApplyBrakes();
    rightESC.ApplyBrakes();
}
