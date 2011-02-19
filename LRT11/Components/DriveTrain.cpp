#include "DriveTrain.h"

DriveTrain::DriveTrain()
    : Component()
    , encoders(DriveEncoders::GetInstance())
    , left(RobotConfig::CAN_DRIVE_LEFTA, RobotConfig::CAN_DRIVE_LEFTB, encoders.GetLeftEncoder(), "left")
    , right(RobotConfig::CAN_DRIVE_RIGHTA, RobotConfig::CAN_DRIVE_RIGHTB, encoders.GetRightEncoder(), "right")
    , dbsDrive(left, right, false)
    , robotDrive(left, right, encoders, dbsDrive)
{

}

DriveTrain::~DriveTrain()
{

}

void DriveTrain::Output()
{
    left.UpdateOutput();
    right.UpdateOutput();
    robotDrive.SetBrakeLeft(action.driveTrain.brakeLeft);
    robotDrive.SetBrakeRight(action.driveTrain.brakeRight);
    robotDrive.ArcadeDrive(action.driveTrain.rawForward, action.driveTrain.rawTurn);
}
