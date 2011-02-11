#include "PositionDriveTrain.h"

PositionDriveTrain::PositionDriveTrain()
    : Component()
    , encoders(DriveEncoders::GetInstance())
    , left(RobotConfig::CAN_DRIVE_LEFT, encoders.GetLeftEncoder(), "left")
    , right(RobotConfig::CAN_DRIVE_RIGHT, encoders.GetRightEncoder(), "right")
    , dbsDrive(left, right, false)
    , robotDrive(left, right, encoders, dbsDrive)
    , positionDrive(robotDrive)
{

}

PositionDriveTrain::~PositionDriveTrain()
{

}

void PositionDriveTrain::Output()
{
    if(action.positionTrain.shouldMoveDistance)
    {
        positionDrive.MoveInches(action.positionTrain.moveDistance);
        action.positionTrain.done = positionDrive.MoveDistanceOutput();
    }
    // interlock; can't move distance and turn simultaneously
    else if(action.positionTrain.shouldTurnAngle)
    {
        positionDrive.TurnAngle(action.positionTrain.turnAngle, action.positionTrain.pivotLeft,
                action.positionTrain.pivotRight);
        action.positionTrain.done = positionDrive.TurnAngleOutput();
    }
}
