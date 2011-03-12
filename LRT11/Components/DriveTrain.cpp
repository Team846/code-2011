#include "DriveTrain.h"

DriveTrain::DriveTrain()
    : Component()
    , encoders(DriveEncoders::GetInstance())
#ifdef LRT_ROBOT_2011
    , left(RobotConfig::CAN_DRIVE_LEFT_A, RobotConfig::CAN_DRIVE_LEFT_B,
            encoders.GetLeftEncoder(), "left")
    , right(RobotConfig::CAN_DRIVE_RIGHT_A, RobotConfig::CAN_DRIVE_RIGHT_B,
            encoders.GetRightEncoder(), "right")
#else
    , left(RobotConfig::CAN_DRIVE_LEFT, encoders.GetLeftEncoder(), "left")
    , right(RobotConfig::CAN_DRIVE_RIGHT, encoders.GetRightEncoder(), "right")
#endif
    , dbsDrive(left, right, false)
    , robotDrive(left, right, encoders, dbsDrive)
{

}

DriveTrain::~DriveTrain()
{

}

void DriveTrain::Output()
{
    // rate train (normal joystick driving) and position train
    // can't run simultaneously
    if(action.positionTrain.enabled)
        return;

    left.UpdateOutput();
    right.UpdateOutput();

    robotDrive.SetClosedLoopEnabled(action.driveTrain.usingClosedLoop);

    robotDrive.SetBrakeLeft(action.driveTrain.brakeLeft);
    robotDrive.SetBrakeRight(action.driveTrain.brakeRight);

    robotDrive.ArcadeDrive(action.driveTrain.rawForward, action.driveTrain.rawTurn);
}
