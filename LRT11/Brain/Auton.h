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
        CONFIGURE,
        DRIVE,
        WAIT_FOR_DRIVE,
        BACK_UP,
        WAIT_FOR_BACK_UP,
        MOVE_LIFT_UP,
        WAIT_FOR_MOVE_LIFT_UP,
        ROTATE_ROLLER,
        RELEASE_ROLLER,
        WAIT_FOR_RELEASE_ROLLER,
        DONE
    } state = CONFIGURE;

    static int counter = 0;
    static float targetDistance = 0;

    if(wasDisabled)
        state = CONFIGURE;

    switch(state)
    {
    case CONFIGURE:
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        state = DRIVE;
        break;

    case DRIVE:
        action.driveTrain.usingClosedLoop = false;
        action.driveTrain.rawForward = 0.5;
        action.driveTrain.rawTurn = 0.0;

        state = WAIT_FOR_DRIVE;
        counter = 0;
        break;

    case WAIT_FOR_DRIVE:
        // after 1s
        if(++counter > 25)
        {
            if(driveEncoders.GetNormalizedLowGearForwardSpeed()
                    < action.driveTrain.rawForward - 0.2)
            {
                action.driveTrain.rawForward = 0.0;
                action.driveTrain.rawTurn = 0.0;
                state = BACK_UP;
            }
        }
        break;

    case BACK_UP:
        action.driveTrain.rawForward = -0.2;
        action.driveTrain.rawTurn = 0.0;

        targetDistance = driveEncoders.GetRobotDist() - 8; // inches
        counter = 0;
        state = WAIT_FOR_BACK_UP;
        break;

    case WAIT_FOR_BACK_UP:
        AsynchronousPrinter::Printf("Moving Back; D: %.2f, TD: %.2f, E: %.2f\n",
                driveEncoders.GetRobotDist(), targetDistance,
                driveEncoders.GetRobotDist() - targetDistance);
        if(driveEncoders.GetRobotDist() < targetDistance)
        {
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.rawTurn = 0.0;
            action.driveTrain.usingClosedLoop = true;
            counter = 0;
            state = MOVE_LIFT_UP;
        }
        break;


    case MOVE_LIFT_UP:
        action.lift.givenCommand = true;
        if(ds.GetDigitalIn(8))
            action.lift.highRow = true; // going from the side
        else
            action.lift.highRow = false; // going from the side

        action.lift.preset = action.lift.HIGH_PEG;
        action.lift.manualMode = false;
        state = WAIT_FOR_MOVE_LIFT_UP;
        break;

    case WAIT_FOR_MOVE_LIFT_UP:
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
            {
                state = ROTATE_ROLLER;
                AsynchronousPrinter::Printf("Entering done!\n");
            }
            else
                state = DONE;
            counter = 0; // reset timer
        }
        break;

    case ROTATE_ROLLER:
        action.roller.rotateUpward = false;
        action.roller.state = action.roller.ROTATING;

        if(++counter >= 25)
            state = RELEASE_ROLLER;
        break;

    case RELEASE_ROLLER:
        action.roller.automated = true;
        action.roller.commenceAutomation = true;
        state = WAIT_FOR_RELEASE_ROLLER;
        counter = 0;
        break;

    case WAIT_FOR_RELEASE_ROLLER:
        AsynchronousPrinter::Printf("Waiting for release roller\n");
        action.roller.commenceAutomation = false;

        // one second of reversing the roller
        if(++counter >= 75)
        {
            // stop automating; stop rollers
            action.roller.automated = false;
            state = DONE;

            counter = 0; // reset timer
        }
        break;

    case DONE:
        // do nothing
        break;

    }
}
