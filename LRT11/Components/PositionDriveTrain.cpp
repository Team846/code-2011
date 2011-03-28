//#include "PositionDriveTrain.h"
//
//PositionDriveTrain::PositionDriveTrain()
//    : Component()
//    , encoders(DriveEncoders::GetInstance())
//#ifdef LRT_ROBOT_2011
//    , left(RobotConfig::CAN_DRIVE_LEFT_A, RobotConfig::CAN_DRIVE_LEFT_B,
//            encoders.GetLeftEncoder(), "left")
//    , right(RobotConfig::CAN_DRIVE_RIGHT_A, RobotConfig::CAN_DRIVE_RIGHT_B,
//            encoders.GetRightEncoder(), "right")
//#else
//    , left(RobotConfig::CAN_DRIVE_LEFT, encoders.GetLeftEncoder(), "left")
//    , right(RobotConfig::CAN_DRIVE_RIGHT, encoders.GetRightEncoder(), "right")
//#endif
//    , dbsDrive(left, right, false)
//    , robotDrive(left, right, encoders, dbsDrive)
//    , positionDrive(robotDrive)
//{
//
//}
//
//PositionDriveTrain::~PositionDriveTrain()
//{
//
//}
//
//void PositionDriveTrain::Output()
//{
////    // only run position train if enabled and the abort button hasn't
////    // been pressed
////    if(!action.positionTrain.enabled || action.master.abort)
////        return;
////
////    positionDrive.SetClosedLoopEnabled(action.positionTrain.usingClosedLoop);
////
////    if(action.positionTrain.shouldMoveDistance)
////    {
////        AsynchronousPrinter::Printf("Moving inches\n");
////        positionDrive.MoveInches(action.positionTrain.moveDistance);
////        action.positionTrain.shouldMoveDistance = false;
////
////        // should only output move distance on a subsequent call of output
////        action.positionTrain.shouldOutputMoveDistance = true;
////        action.positionTrain.shouldOutputTurnAngle = false;
////    }
////    // interlock; can't move distance and turn simultaneously
////    else if(action.positionTrain.shouldTurnAngle)
////    {
////        positionDrive.TurnAngle(action.positionTrain.turnAngle, action.positionTrain.pivotLeft,
////                action.positionTrain.pivotRight);
////        action.positionTrain.shouldTurnAngle = false;
////
////        // should only output turn angle on a subsequent call of output
////        action.positionTrain.shouldOutputTurnAngle = true;
////        action.positionTrain.shouldOutputMoveDistance = false;
////    }
////
////    if(action.positionTrain.shouldOutputMoveDistance)
////    {
////        // output and store whether movement is done
////        action.positionTrain.done = positionDrive.MoveDistanceOutput();
////        AsynchronousPrinter::Printf("Outputting move distance\n");
////    }
////    else if(action.positionTrain.shouldOutputTurnAngle)
////        // output and store whether movement is done
////        action.positionTrain.done = positionDrive.TurnAngleOutput();
//}
