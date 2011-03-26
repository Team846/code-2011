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
    , positionDrive(robotDrive)
{

}

DriveTrain::~DriveTrain()
{

}

void DriveTrain::Output()
{
    //code common to both position and speed modes
    left.UpdateOutput();
    right.UpdateOutput();
    robotDrive.SetClosedLoopEnabled(action.driveTrain.usingClosedLoop);

    robotDrive.SetBrakeLeft(action.driveTrain.brakeLeft);
    robotDrive.SetBrakeRight(action.driveTrain.brakeRight);

    robotDrive.SetHighGear(action.shifter.gear == action.shifter.HIGH_GEAR);

    if(action.driveTrain.resetFwd)
        positionDrive.ResetFwd();
    if(action.driveTrain.resetTurn)
        positionDrive.ResetTurn();

    action.driveTrain.resetFwd = false;
    action.driveTrain.resetTurn = true;

    // rate train (normal joystick driving) and position train
    // can't run simultaneously
    if(action.driveTrain.mode == action.driveTrain.POSITION)
    {
        action.driveTrain.done = positionDrive.Output
                (action.driveTrain.distanceSetPoint,
                        action.driveTrain.bearingSetPoint);
    }
    else
    {
        // abort overrides everything; stop if abort button is pushed
        if(action.master.abort)
            robotDrive.ArcadeDrive(0.0, 0.0);
        else if(action.driveTrain.thirdGear)
            robotDrive.ArcadeDrive(action.driveTrain.rawForward / 2.0, action.driveTrain.rawTurn / 2.0);
        else
            robotDrive.ArcadeDrive(action.driveTrain.rawForward, action.driveTrain.rawTurn);
    }



}
