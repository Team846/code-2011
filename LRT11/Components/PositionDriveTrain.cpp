#include "PositionDriveTrain.h"

PositionDriveTrain::PositionDriveTrain()
    : Component()
    , encoders(DriveEncoders::GetInstance())
    , left(RobotConfig::CAN_DRIVE_LEFTA, RobotConfig::CAN_DRIVE_LEFTB, encoders.GetLeftEncoder(), "left")
    , right(RobotConfig::CAN_DRIVE_RIGHTA, RobotConfig::CAN_DRIVE_RIGHTB, encoders.GetRightEncoder(), "right")
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
        action.positionTrain.shouldOutputTurnAngle = false;
        action.positionTrain.shouldOutputMoveDistance = true;
        positionDrive.MoveInches(action.positionTrain.moveDistance);
        action.positionTrain.shouldMoveDistance = false;
    }
    // interlock; can't move distance and turn simultaneously
    else if(action.positionTrain.shouldTurnAngle)
    {
        action.positionTrain.shouldOutputMoveDistance = false;
        action.positionTrain.shouldOutputTurnAngle = true;
        positionDrive.TurnAngle(action.positionTrain.turnAngle, action.positionTrain.pivotLeft,
                action.positionTrain.pivotRight);
        action.positionTrain.shouldTurnAngle = false;
    }

    if(action.positionTrain.shouldOutputMoveDistance)
    {
        //AsynchronousPrinter::Printf("Outputting move distance\n");
        action.positionTrain.done = positionDrive.MoveDistanceOutput();
//        action.positionTrain.shouldOutputMoveDistance = !action.positionTrain.done;
//
//        if(action.positionTrain.done)
//            positionDrive.Stop();
    }
    else if(action.positionTrain.shouldOutputTurnAngle)
    {
        action.positionTrain.done = positionDrive.TurnAngleOutput();
//        action.positionTrain.shouldOutputTurnAngle = !action.positionTrain.done;
//
//        if(action.positionTrain.done)
//            positionDrive.Stop();
    }
}
