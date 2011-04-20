#include "ModifiedDriveTrain.h"
#include <math.h>

ModifiedDriveTrain::ModifiedDriveTrain()
    : Component()
    , driveEncoders(DriveEncoders::GetInstance())
    , closedRateTrain()
    , closedPositionTrain(closedRateTrain)
#ifdef LRT_ROBOT_2011
    , leftESC(RobotConfig::CAN_DRIVE_LEFT_A, RobotConfig::CAN_DRIVE_LEFT_B,
            DriveEncoders::GetInstance().GetLeftEncoder(), "left")
    , rightESC(RobotConfig::CAN_DRIVE_RIGHT_A, RobotConfig::CAN_DRIVE_RIGHT_B,
            DriveEncoders::GetInstance().GetRightEncoder(), "right")
#else
    // TODO fix initialization
    , leftESC(RobotConfig::CAN_DRIVE_LEFT, driveEncoders.GetLeftEncoder(), "left")
    , rightESC(RobotConfig::CAN_DRIVE_RIGHT, driveEncoders.GetInstance().GetRightEncoder(), "right")
    , config(Config::GetInstance())
#endif
{
#warning "Set number of cycles to synchronize for"
    cyclesToSynchronize = config.Get<int>("Drivetrain.CyclesToSynchronize", 35);
    synchronizedCyclesLeft = 0;
}

ModifiedDriveTrain::~ModifiedDriveTrain()
{

}

void ModifiedDriveTrain::Output()
{
    DriveCommand drive;

    closedRateTrain.SetHighGear(action.shifter.gear == action.shifter.HIGH_GEAR);
    closedRateTrain.SetClosedLoopEnabled(action.driveTrain.rate.usingClosedLoop);

    // calculate left duty cycle, right duty cycle, left brake, and
    // right brake based off of joystick inputs and mode
    switch(action.driveTrain.mode)
    {
    case SPEED:
        if(action.driveTrain.rate.thirdGear)
            // scale raw turn to a max of 0.3
            drive = closedRateTrain.Drive(action.driveTrain.rate.rawForward, action.driveTrain.rate.rawTurn * 0.3);
        else
            drive = closedRateTrain.Drive(action.driveTrain.rate.rawForward, action.driveTrain.rate.rawTurn);
        break;

    case POSITION:
        if(action.driveTrain.position.givenCommand)
        {
            if(action.driveTrain.position.shouldMoveDistance)
            {
                AsynchronousPrinter::Printf("Move distance command");
                closedPositionTrain.SetMovePosition(action.driveTrain.position.distanceSetPoint);
            }
            else if(action.driveTrain.position.shouldTurnAngle)
            {
                AsynchronousPrinter::Printf("Turn angle command");
                closedPositionTrain.SetTurnAngle(action.driveTrain.position.turnSetPoint);
            }

            action.driveTrain.position.givenCommand = false;
            action.driveTrain.position.shouldMoveDistance = false;
            action.driveTrain.position.shouldTurnAngle = false;
        }

        drive = closedPositionTrain.Drive(action.driveTrain.position.maxFwdSpeed,
                action.driveTrain.position.maxTurnSpeed);
        break;

    case DISTANCE:
        if(action.driveTrain.distance.givenCommand)
        {
            closedPositionTrain.SetMoveDistance(action.driveTrain.distance.distanceSetPoint);
            action.driveTrain.distance.givenCommand = false;
        }

        CLPositionCommand command =
            closedPositionTrain.DriveAtLeastDistance(action.driveTrain.distance.distanceDutyCycle);
        action.driveTrain.distance.done = command.done;

        drive = command.drive;
        break;
    case SYNCHRONIZING:
        synchronizedCyclesLeft = cyclesToSynchronize;
        break;
    }

    if(--synchronizedCyclesLeft > 0)
    {
        drive.rightCommand.dutyCycle = GetSynchronizedSpeed(driveEncoders.GetNormalizedRightMotorSpeed());
        drive.leftCommand.dutyCycle = GetSynchronizedSpeed(driveEncoders.GetNormalizedLeftMotorSpeed());
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

    if(action.wasDisabled)
    {
        leftESC.ResetCache();
        rightESC.ResetCache();
    }
}

float ModifiedDriveTrain::GetSynchronizedSpeed(float motorSpeed) //motor speed refers to the speed of the motor if it were engaged
{
    float absMotorSpeed = fabs(motorSpeed);
    if(absMotorSpeed < 1E-4)
        return 0.1; //We can't shift without moving so if we are stopped we spin forward at 10%
    else if(absMotorSpeed < .10)
        return 0.10 * Util::Sign<float>(motorSpeed); //If we are moving very slowly apply 10% power in the direction of movement to ensure the motor actually spins

    return motorSpeed; //Otherwise just spin the motor close the the speed of the output shaft
}
