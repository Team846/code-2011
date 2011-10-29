#include "SynchronizedDrive.h"

SynchronizedDrive::SynchronizedDrive()
: leftEncoder(DriveEncoders::GetInstance().GetLeftEncoder())
, rightEncoder(DriveEncoders::GetInstance().GetRightEncoder())
, driveEncoders(DriveEncoders::GetInstance())
{
	
}

SynchronizedDrive::~SynchronizedDrive()
{
	
}

void SynchronizedDrive::Configure()
{
	
}

DriveCommand SynchronizedDrive::Drive()
{
	DriveCommand drive;
	drive.rightCommand.dutyCycle = driveEncoders.NormalizedOpposingGearMotorSpeed(driveEncoders.GetRightEncoder());
	drive.rightCommand.dutyCycle = EnsureFloorSpeed(drive.rightCommand.dutyCycle ); //limits to not less than 10%
	
	drive.leftCommand.dutyCycle  = driveEncoders.NormalizedOpposingGearMotorSpeed(driveEncoders.GetLeftEncoder());
	drive.leftCommand.dutyCycle = EnsureFloorSpeed(drive.leftCommand.dutyCycle );
	
	drive.rightCommand.dutyCycle *= 1.0;  //Maybe reduce power since the motors are unloaded.
	drive.leftCommand.dutyCycle *= 1.0;
	
	drive.shouldBrake = false;
	return drive;
}

float SynchronizedDrive::EnsureFloorSpeed(float motorSpeed)
{
    float absMotorSpeed = fabs(motorSpeed);
    
    // in case where sign returns zero
    if(absMotorSpeed < 1E-4) 
        return 0.1; //We can't shift without moving so if we are stopped we apply 10% forward power (not enough to move the robot)
    else if(absMotorSpeed < .10)
        return 0.10 * Util::Sign<float>(motorSpeed); //If we are moving very slowly apply 10% power in the direction of movement to ensure the motor actually spins
    else 
    	return motorSpeed; //Otherwise just spin the motor close the the speed of the output shaft

}
