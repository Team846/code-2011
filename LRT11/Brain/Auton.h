#include "Brain.h"

void Brain::Auton()
{
//    // TODO remove return statement after auton is legitimate
//    return;
//
//    // robot starts at either a side or the middle
//    static enum
//    {
//        SIDE,
//        MIDDLE,
//        DUMB
//    } startPosition;
//
//    // position determined by digital inputs
//    if(ds.GetDigitalIn(1))
//        startPosition = SIDE;
//    else if(ds.GetDigitalIn(2))
//        startPosition = MIDDLE;
//    else
//        startPosition = DUMB;
//
//    // call state-specific methods
//    switch(startPosition)
//    {
//    case SIDE:
//        Side();
//        break;
//    case MIDDLE:
//        Middle(ds.GetDigitalIn(2) ? 2 : 1);
//        break;
//    case DUMB:
    TeleopRoller();
    EncoderAuton();
//        break;
//    }
//
//    // includes automated routines such as line sensing and
//    // dead-reckoning autonomous mode
////    AutomatedRoutines();
}

void Brain::EncoderAuton()
{
    static enum
    {
        SET_COMMAND,
        DRIVE_FORWARD,
        STALL_DETECTION,
        SET_SECOND_COMMAND,
        STEP_BACK,
        MOVE_LIFT_UP,
        WAIT_FOR_MOVE_LIFT_UP,
        ROTATE_ROLLER,
        RELEASE_ROLLER,
        WAIT_FOR_RELEASE_ROLLER,
        MOVE_LIFT_DOWN,
        WAIT_FOR_MOVE_LIFT_DOWN,
        SET_THIRD_COMMAND,
        DRIVE_BACK,
        TURN_AROUND,
        IDLE
    } state = SET_COMMAND;

    static int timer = 0;

    if(wasDisabled)
    {
        timer = 0;
        state = SET_COMMAND;
    }

    switch(state)
    {
    case SET_COMMAND:
        action.driveTrain.mode = action.driveTrain.DISTANCE;
        action.driveTrain.distance.givenCommand = true;

        action.driveTrain.distance.distanceSetPoint = 15.0 * 12; // 15 feet
        action.driveTrain.distance.distanceDutyCycle = 0.5;

        action.driveTrain.distance.done = false;
        // arm should stay in top position
        action.arm.state = action.arm.PRESET_TOP;

        // low gear driving
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        state = DRIVE_FORWARD;
        break;

    case DRIVE_FORWARD:
        if(action.driveTrain.distance.done)
        {
            timer = 0;
            state = STALL_DETECTION;
        }
        break;

    case STALL_DETECTION:
        action.driveTrain.mode = action.driveTrain.RATE;
        action.driveTrain.rate.usingClosedLoop = false;

        action.driveTrain.rate.rawForward = 0.2;
        action.driveTrain.rate.rawTurn = 0.0;

        // robot has speed up enough to start stall detection
        if(driveEncoders.GetNormalizedLowGearForwardSpeed() > 0.1)
            timer = 51; // greater than 50; see if statement below

        if(++timer > 50)
        {
            if(driveEncoders.GetNormalizedLowGearForwardSpeed()
                    < 0.05)
                state = SET_SECOND_COMMAND;
        }
        break;

    case SET_SECOND_COMMAND:
        action.driveTrain.rate.usingClosedLoop = true;
        action.driveTrain.rate.rawForward = 0.0;
        action.driveTrain.rate.rawTurn = 0.0;

        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = true;
        action.driveTrain.position.shouldTurnAngle = false;

        action.driveTrain.position.distanceSetPoint = -3.0; // 3 inches back
        action.driveTrain.position.turnSetPoint = 0.0;

        action.driveTrain.position.maxFwdSpeed = 0.15;
        action.driveTrain.position.maxTurnSpeed = 1.0;

        timer = 0;
        state = STEP_BACK;
        break;

    case STEP_BACK:
        // wait one second for driving to finish
        if(++timer > 50)
            state = MOVE_LIFT_UP;
        break;

    case MOVE_LIFT_UP:
//        AsynchronousPrinter::Printf("Moving lift up\n");
        action.lift.givenCommand = true;
        // depends on if the robot is in the middle or on the side
        action.lift.highRow = DriverStation::GetInstance()->GetDigitalIn(2);

        action.lift.preset = action.lift.HIGH_PEG;
        action.lift.manualMode = false;
        state = WAIT_FOR_MOVE_LIFT_UP;
        break;

    case WAIT_FOR_MOVE_LIFT_UP:
//        AsynchronousPrinter::Printf("Waiting for move lift up\n");
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = ROTATE_ROLLER;
//            else if(action.lift.doneState == action.lift.FAILURE)
//            {
//                AsynchronousPrinter::Printf("Failure\n");
//                state = IDLE;
//            }
            else
                state = IDLE;
            timer = 0; // reset timer
        }
        break;

    case ROTATE_ROLLER:
//        AsynchronousPrinter::Printf("Rotate roller\n");
        action.roller.rotateUpward = false;
        action.roller.state = action.roller.ROTATING;

        if(++timer >= 75)
            state = RELEASE_ROLLER;
        break;

    case RELEASE_ROLLER:
//        AsynchronousPrinter::Printf("Release roller\n");
        action.roller.automated = true;
        action.roller.commenceAutomation = true;
        state = WAIT_FOR_RELEASE_ROLLER;
        timer = 0;
        break;

    case WAIT_FOR_RELEASE_ROLLER:
//        AsynchronousPrinter::Printf("Wait for release roller\n");
        action.roller.commenceAutomation = false;

        // half second of reversing the roller
        if(++timer >= 25)
        {
            // stop automating; stop rollers
            action.roller.automated = false;
            state = MOVE_LIFT_DOWN;

            timer = 0; // reset timer
        }
        break;

    case MOVE_LIFT_DOWN:
//        AsynchronousPrinter::Printf("Move lift down\n");
        action.lift.givenCommand = true;
        // depends on if the robot is in the middle or on the side
        action.lift.highRow = inputs.IsHighRow();

        action.lift.preset = action.lift.LOW_PEG;
        action.lift.manualMode = false;
        state = WAIT_FOR_MOVE_LIFT_DOWN;
        break;

    case WAIT_FOR_MOVE_LIFT_DOWN:
//        AsynchronousPrinter::Printf("Wait for move lift down\n");
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = SET_THIRD_COMMAND;
//                state = IDLE;
            else
                state = IDLE;
            timer = 0; // reset timer
        }
        break;

    case SET_THIRD_COMMAND:
        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = true;
        action.driveTrain.position.shouldTurnAngle = false;

        action.driveTrain.position.distanceSetPoint = -14.5 * 12; // 14.5 feet back
        action.driveTrain.position.turnSetPoint = 0.0;

        action.driveTrain.position.maxFwdSpeed = 0.5;
        action.driveTrain.position.maxTurnSpeed = 1.0;

        timer = 0;
        state = DRIVE_BACK;
        break;

    case DRIVE_BACK:
        if(++timer > 150)
            state = TURN_AROUND;
        break;

    case TURN_AROUND:
        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = false;
        action.driveTrain.position.shouldTurnAngle = true;

        action.driveTrain.position.distanceSetPoint = 0.0;
        action.driveTrain.position.turnSetPoint = 180.0; // 180 degrees

        action.driveTrain.position.maxFwdSpeed = 1.0;
        action.driveTrain.position.maxTurnSpeed = 1.0;

        state = IDLE;
        break;

    case IDLE:
//        AsynchronousPrinter::Printf("Idle\n");
        // wait for turning to complete and do nothing
        break;
    }

//    static enum
//    {
//        CONFIGURE,
//        DRIVE,
//        WAIT_FOR_DRIVE,
//        BACK_UP,
//        WAIT_FOR_BACK_UP,
//        MOVE_LIFT_UP,
//        WAIT_FOR_MOVE_LIFT_UP,
//        ROTATE_ROLLER,
//        RELEASE_ROLLER,
//        WAIT_FOR_RELEASE_ROLLER,
//        DONE
//    } state = CONFIGURE;
//
//    static int counter = 0;
//    static float targetDistance = 0;
//
//    if(wasDisabled)
//        state = CONFIGURE;
//
//    switch(state)
//    {
//    case CONFIGURE:
//        action.shifter.gear = action.shifter.LOW_GEAR;
//        action.shifter.force = true;
//
//        state = DRIVE;
//        break;
//
//    case DRIVE:
//        action.driveTrain.usingClosedLoop = false;
//        action.driveTrain.rawForward = 1.0;
//        action.driveTrain.rawTurn = 0.0;
//
//        state = WAIT_FOR_DRIVE;
//        counter = 0;
//        break;
//
//    case WAIT_FOR_DRIVE:
//        // after 1s
//        if(++counter > 25)
//        {
//            if(driveEncoders.GetNormalizedLowGearForwardSpeed()
//                    < action.driveTrain.rawForward - 0.2)
//            {
//                action.driveTrain.rawForward = 0.0;
//                action.driveTrain.rawTurn = 0.0;
//                state = BACK_UP;
//            }
//        }
//        break;
//
//    case BACK_UP:
//        action.driveTrain.rawForward = -0.2;
//        action.driveTrain.rawTurn = 0.0;
//
//        targetDistance = driveEncoders.GetRobotDist() - 8; // inches
//        counter = 0;
//        state = WAIT_FOR_BACK_UP;
//        break;
//
//    case WAIT_FOR_BACK_UP:
//        AsynchronousPrinter::Printf("Moving Back; D: %.2f, TD: %.2f, E: %.2f\n",
//                driveEncoders.GetRobotDist(), targetDistance,
//                driveEncoders.GetRobotDist() - targetDistance);
//        if(driveEncoders.GetRobotDist() < targetDistance)
//        {
//            action.driveTrain.rawForward = 0.0;
//            action.driveTrain.rawTurn = 0.0;
//            action.driveTrain.usingClosedLoop = true;
//            counter = 0;
//            state = MOVE_LIFT_UP;
//        }
//        break;
//
//
//    case MOVE_LIFT_UP:
//        action.lift.givenCommand = true;
//        if(ds.GetDigitalIn(8))
//            action.lift.highRow = true; // going from the side
//        else
//            action.lift.highRow = false; // going from the side
//
//        action.lift.preset = action.lift.HIGH_PEG;
//        action.lift.manualMode = false;
//        state = WAIT_FOR_MOVE_LIFT_UP;
//        break;
//
//    case WAIT_FOR_MOVE_LIFT_UP:
//        if(action.lift.doneState != action.lift.STALE) // message is available
//        {
//            if(action.lift.doneState == action.lift.SUCCESS)
//            {
//                state = ROTATE_ROLLER;
//                AsynchronousPrinter::Printf("Entering done!\n");
//            }
//            else
//                state = DONE;
//            counter = 0; // reset timer
//        }
//        break;
//
//    case ROTATE_ROLLER:
//        action.roller.rotateUpward = false;
//        action.roller.state = action.roller.ROTATING;
//
//        if(++counter >= 25)
//            state = RELEASE_ROLLER;
//        break;
//
//    case RELEASE_ROLLER:
//        action.roller.automated = true;
//        action.roller.commenceAutomation = true;
//        state = WAIT_FOR_RELEASE_ROLLER;
//        counter = 0;
//        break;
//
//    case WAIT_FOR_RELEASE_ROLLER:
//        AsynchronousPrinter::Printf("Waiting for release roller\n");
//        action.roller.commenceAutomation = false;
//
//        // one second of reversing the roller
//        if(++counter >= 75)
//        {
//            // stop automating; stop rollers
//            action.roller.automated = false;
//            state = DONE;
//
//            counter = 0; // reset timer
//        }
//        break;
//
//    case DONE:
//        // do nothing
//        break;
//
//    }
}
