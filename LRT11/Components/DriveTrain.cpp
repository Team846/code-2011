#include "DriveTrain.h"

DriveTrain::DriveTrain()
    : Component()
    , encoders(DriveEncoders::GetInstance())
    , left(RobotConfig::CAN_DRIVE_LEFT, encoders.GetLeftEncoder(), "left")
    , right(RobotConfig::CAN_DRIVE_RIGHT, encoders.GetRightEncoder(), "right")
    , dbsDrive(left, right, false)
    , robotDrive(left, right, encoders, dbsDrive)
{

}

DriveTrain::~DriveTrain()
{

}

void DriveTrain::Output()
{
    robotDrive.SetBrakeLeft(action.driveTrain.brakeLeft);
    robotDrive.SetBrakeRight(action.driveTrain.brakeRight);
    robotDrive.ArcadeDrive(action.driveTrain.rawForward, action.driveTrain.rawTurn);
}
