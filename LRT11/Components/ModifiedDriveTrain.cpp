#include "ModifiedDriveTrain.h"

ModifiedDriveTrain::ModifiedDriveTrain()
    : Component()
    , closedRateTrain()
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

void ModifiedDriveTrain::Output()
{
    // calculate left duty cycle, right duty cycle, left brake, and
    // right brake based off of joystick inputs
    DriveCommand drive = closedRateTrain.Drive(action.driveTrain.rawForward,
            action.driveTrain.rawTurn);

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