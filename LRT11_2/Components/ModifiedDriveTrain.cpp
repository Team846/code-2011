#include "ModifiedDriveTrain.h"
#include "..\Config\Config.h"
#include "DriveTrain\VectorDrive.h"
#include "DriveTrain\CLPositionDriveTrain.h"
#include "DriveTrain\SynchronizedDrive.h"
#include "..\Sensors\DriveEncoders.h"
#include "..\Jaguar\Esc.h"
#include "..\Config\RobotConfig.h"
#include "..\ActionData\DriveAction.h"
#include "..\ActionData\ShifterAction.h"
#include <math.h>

ModifiedDriveTrain::ModifiedDriveTrain()
    : Component()
    , driveEncoders(DriveEncoders::GetInstance()) //TODO: If this is a singleton, why create it here? -dg
    , config(Config::GetInstance())
{
    closedRateTrain 	= new VectorDrive();
    closedPositionTrain = new CLPositionDriveTrain(*closedRateTrain);
    synchronizedDrive 	= new SynchronizedDrive();
    
    leftESC = new Esc(RobotConfig::CAN::DRIVE_LEFT_A, RobotConfig::CAN::DRIVE_LEFT_B,
            driveEncoders.GetLeftEncoder(), "left");
    rightESC = new Esc(RobotConfig::CAN::DRIVE_RIGHT_A, RobotConfig::CAN::DRIVE_RIGHT_B,
            driveEncoders.GetRightEncoder(), "right");

    Configure();
    synchronizedCyclesLeft = 0;

//    leftESC->CollectCurrent();
//    rightESC->CollectCurrent();
    printf("Constructed Drive (ModifiedDriveTrain)\n");
}

ModifiedDriveTrain::~ModifiedDriveTrain()
{
    driveEncoders.~DriveEncoders();

    delete closedRateTrain;
    delete closedPositionTrain;
    delete leftESC;
    delete rightESC;
}

void ModifiedDriveTrain::Configure()
{
    cyclesToSynchronize = config.Get<int>("Drivetrain", "CyclesToSynchronize", 60);
}

void ModifiedDriveTrain::Output()
{
    DriveCommand drive;

//    static int cycleCount = 0;
//    if(++cycleCount % 10 == 0)
//    {
//        AsynchronousPrinter::Printf("Left: %6.3f ", leftESC->GetCurrent());
//        AsynchronousPrinter::Printf("Right: %6.3f\n", rightESC->GetCurrent());
//    }

    closedRateTrain->SetHighGear(action.shifter->gear == ACTION::GEARBOX::HIGH_GEAR);
    closedRateTrain->SetClosedLoopEnabled(action.driveTrain->rate.usingClosedLoop);

//    closedRateTrain->SetClosedLoopEnabled(false);


    // calculate left duty cycle, right duty cycle, left brake, and
    // right brake based off of joystick inputs and mode
    switch(action.driveTrain->mode)
    {
    case ACTION::DRIVETRAIN::SPEED:
//        if(action.driveTrain->rate.thirdGear)
//            // scale raw turn to a max of 0.3
//            drive = closedRateTrain->Drive(action.driveTrain->rate.rawForward, action.driveTrain->rate.rawTurn * 0.3);
//        else
//            drive = closedRateTrain->Drive(action.driveTrain->rate.rawForward, action.driveTrain->rate.rawTurn);
        break;

    case ACTION::DRIVETRAIN::POSITION:
        if(action.driveTrain->position.givenCommand)
        {
            if(action.driveTrain->position.shouldMoveDistance)
            {
                AsynchronousPrinter::Printf("Move distance command");
                closedPositionTrain->SetMovePosition(action.driveTrain->position.distanceSetPoint);
            }
            else if(action.driveTrain->position.shouldTurnAngle)
            {
                AsynchronousPrinter::Printf("Turn angle command");
                closedPositionTrain->SetTurnAngle(action.driveTrain->position.turnSetPoint);
            }

            action.driveTrain->position.givenCommand = false;
            action.driveTrain->position.shouldMoveDistance = false;
            action.driveTrain->position.shouldTurnAngle = false;
        }

        drive = closedPositionTrain->Drive(action.driveTrain->position.maxFwdSpeed,
                action.driveTrain->position.maxTurnSpeed);
        break;

    case ACTION::DRIVETRAIN::DISTANCE:
        if(action.driveTrain->distance.givenCommand)
        {
            closedPositionTrain->SetMoveDistance(action.driveTrain->distance.distanceSetPoint);
            action.driveTrain->distance.givenCommand = false;
        }

        CLPositionCommand command =
            closedPositionTrain->DriveAtLeastDistance(action.driveTrain->distance.distanceDutyCycle);
        action.driveTrain->distance.done = command.done;

        drive = command.drive;
        break;
    case ACTION::DRIVETRAIN::SYNCHRONIZING:
        synchronizedCyclesLeft = cyclesToSynchronize;  //set shift timer from value in config file
        break;
    case ACTION::DRIVETRAIN::VECTOR:
    	drive = closedRateTrain->Drive(action.driveTrain->vector.heading, action.driveTrain->vector.fwd);
    	break;
    }

    if(synchronizedCyclesLeft > 0)
    {
	    drive = synchronizedDrive->Drive();
        synchronizedCyclesLeft--;
    }

    // leftDC and rightDC are set to 0 if there is a need to brake;
    // see DitheredBrakeTrain's Drive method
    leftESC->SetDutyCycle(drive.leftCommand.dutyCycle);
    rightESC->SetDutyCycle(drive.rightCommand.dutyCycle);

    if (!drive.shouldBrake)
    {
        leftESC->SetBrake(0);
        rightESC->SetBrake(0);
    }

    // apply brakes only has an effect if SetBrake is called with a
    // non-zero parameter
    leftESC->ApplyBrakes();
    rightESC->ApplyBrakes();

    if(action.wasDisabled) //if the robot was just enabled reset the cache
    {
        leftESC->ResetCache();
        rightESC->ResetCache();
    }
}


string ModifiedDriveTrain::GetName()
{
    return "ModifiedDriveTrain";
}
