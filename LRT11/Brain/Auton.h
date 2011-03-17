#include "Brain.h"

void Brain::Auton()
{
    // TODO remove return statement
    return;

    // robot starts at either a side or the middle
    static enum
    {
        SIDE,
        MIDDLE
    } startPosition;

    // position determined by digital inputs
    if(ds.GetDigitalIn(1))
        startPosition = SIDE;
    else
        startPosition = MIDDLE;

    // call state-specific methods
    switch(startPosition)
    {
    case SIDE:
        Side();
        break;
    case MIDDLE:
        Middle(ds.GetDigitalIn(2) ? 2 : 1);
        break;
    }

    // includes automated routines such as line sensing and
    // dead-reckoning autonomous mode
    AutomatedRoutines();
}

void Brain::Middle(int numberOfTubes)
{
    static enum
    {
        CONFIGURE,

        DRIVE_FORWARD,
        WAIT_FOR_DRIVE_TRAIN,

        FOLLOW_LINE,
        MOVE_LIFT_UP,
        RELEASE_RINGER,
        MOVE_LIFT_DOWN,
        TURN_AROUND
    } state = CONFIGURE;

    switch(state)
    {
    case CONFIGURE:
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        action.positionTrain.enabled = true;
        state = DRIVE_FORWARD;
        break;

    case DRIVE_FORWARD:
        action.positionTrain.shouldMoveDistance = true;
        // placeholder; distance to pegs in inches
        action.positionTrain.moveDistance = 16.75 * 12;

        state = WAIT_FOR_DRIVE_TRAIN;
        break;

    case WAIT_FOR_DRIVE_TRAIN:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            state = FOLLOW_LINE;
        }
        break;

    case FOLLOW_LINE:
        AutomatedFollowLine();
        break;

    case MOVE_LIFT_UP:
        break;

    case RELEASE_RINGER:
        break;

    case MOVE_LIFT_DOWN:
        break;

    case TURN_AROUND:
        break;

    }

    /*

        static bool isOnLeftSide;
        static double followStartDist;
        static enum
        {
            DRIVE_TO_Y,
            FOLLOW_Y,
            MOVE_LIFT_UP1,
            RELEASE_TUBE1,
            MOVE_LIFT_DOWN1,
            TURN_TO_GO_BACK,
            MOVE_BACK,
            TURN_TO_PICKUP_RINGER,
            DRIVE_TO_LINE,
            FOLLOW_LINE,
            MOVE_LIFT_UP2,
            RELEASE_TUBE2,
            MOVE_LIFT_DOWN2,
            MOVE_BACK2,

            WAIT_FOR_DRIVE_TO_Y,
            WAIT_FOR_FOLLOW_Y,
            WAIT_FOR_MOVE_LIFT_UP1,
            WAIT_FOR_RELEASE_TUBE1,
            WAIT_FOR_MOVE_LIFT_DOWN1,
            WAIT_FOR_TURN_TO_GO_BACK,
            WAIT_FOR_MOVE_BACK,
            WAIT_FOR_TURN_TO_PICKUP_RINGER,
            WAIT_FOR_DRIVE_TO_LINE,
            WAIT_FOR_FOLLOW_LINE,
            WAIT_FOR_MOVE_LIFT_UP2,
            WAIT_FOR_RELEASE_TUBE2,
            WAIT_FOR_MOVE_LIFT_DOWN2,
            WAIT_FOR_MOVE_BACK2,

            IDLE

        } state = DRIVE_TO_Y;

        switch(state)
        {
            //Stop a little short (0.65 ft) of the leg of the Y
        case DRIVE_TO_Y:
            // switch to low gear and shift
            action.shifter.gear = action.shifter.LOW_GEAR;
            action.shifter.force = true;

            action.positionTrain.enabled = true;
            action.positionTrain.shouldMoveDistance = true;

            // placeholder; distance to pegs in inches
            action.positionTrain.moveDistance = 16.75 * 12;

            state = WAIT_FOR_DRIVE_TO_Y;
            break;

        case FOLLOW_Y:
            followStartDist = driveEncoders.GetRobotDist();
            state = WAIT_FOR_FOLLOW_Y;
            break;

        case MOVE_LIFT_UP1:
            action.lift.givenCommand = true;
            action.lift.highRow = false;
            action.lift.preset = action.lift.HIGH_PEG;
            state = WAIT_FOR_MOVE_LIFT_UP1;
            break;

        case RELEASE_TUBE1:
            action.roller.rotateUpward = true;
            action.roller.state = action.roller.ROTATING;
            state = WAIT_FOR_RELEASE_TUBE1;
            break;

        case MOVE_LIFT_DOWN1:
            action.lift.givenCommand = true;
            action.lift.highRow = false;
            action.lift.preset = action.lift.LOW_PEG;
            state = WAIT_FOR_MOVE_LIFT_DOWN1;
            break;

        case TURN_TO_GO_BACK:
            action.positionTrain.enabled = true;
            action.positionTrain.shouldTurnAngle = true;

            if(driveEncoders.GetTurnAngle() > 0)  //TODO CHECKME! These may be wrong.
            {
                isOnLeftSide = true;
                action.positionTrain.pivotLeft = true;
                action.positionTrain.turnAngle = -45;
            }
            else
            {
                isOnLeftSide = false;
                action.positionTrain.pivotRight = true;
                action.positionTrain.turnAngle  = 45;
            }

            state = WAIT_FOR_TURN_TO_GO_BACK;
            break;

        case MOVE_BACK:
            action.positionTrain.enabled = true;
            action.positionTrain.shouldMoveDistance = true;

            // placeholder; distance to pegs in inches
            action.positionTrain.moveDistance = -20.5 * 12;

            state = WAIT_FOR_MOVE_BACK;
            break;

        case TURN_TO_PICKUP_RINGER:
            action.positionTrain.enabled = true;
            action.positionTrain.shouldTurnAngle = true;

            if(isOnLeftSide)  //TODO CHECKME! These may be wrong.
                action.positionTrain.turnAngle = 45;
            else
                action.positionTrain.turnAngle  = -45;

            state = WAIT_FOR_TURN_TO_PICKUP_RINGER;
            break;

        case DRIVE_TO_LINE:
            action.positionTrain.enabled = true;
            action.positionTrain.shouldMoveDistance = true;

            // placeholder; distance to pegs in inches
            action.positionTrain.moveDistance = 75;


            state = WAIT_FOR_DRIVE_TO_LINE;
            break;

        case FOLLOW_LINE:
            followStartDist = driveEncoders.GetRobotDist();
            state = WAIT_FOR_FOLLOW_LINE;
            break;

        case MOVE_LIFT_UP2:
            action.lift.givenCommand = true;
            action.lift.highRow = false;
            action.lift.preset = action.lift.HIGH_PEG;
            state = WAIT_FOR_MOVE_LIFT_UP2;
            break;

        case RELEASE_TUBE2:
            action.roller.rotateUpward = true;
            action.roller.state = action.roller.ROTATING;
            state = WAIT_FOR_RELEASE_TUBE2;
            break;

        case MOVE_LIFT_DOWN2:
            action.lift.givenCommand = true;
            action.lift.highRow = false;
            action.lift.preset = action.lift.LOW_PEG;
            state = WAIT_FOR_MOVE_LIFT_DOWN2;
            break;

        case MOVE_BACK2:
            action.positionTrain.enabled = true;
            action.positionTrain.shouldMoveDistance = true;

            action.positionTrain.moveDistance = -24;

            state = WAIT_FOR_MOVE_BACK2;
            break;


        case WAIT_FOR_DRIVE_TO_Y:
            if(action.positionTrain.done)
            {
                action.positionTrain.enabled = false;
                // should move distance is automatically reset

                state = FOLLOW_Y;
            }
            break;

        case WAIT_FOR_FOLLOW_Y:
            AutomatedFollowLine();//Follow the line

            //when close to to the pole disable closed loop and enable stall detection
            if(driveEncoders.GetRobotDist() > 20) //TODO Find the val empirically, maybe reduce speed
                action.driveTrain.usingClosedLoop = false;

            if(driveEncoders.GetNormalizedForwardSpeed() < 0.2) //TODO tweak This means we have crashed into the pole
            {
                action.driveTrain.usingClosedLoop = true;
                action.driveTrain.rawForward = 0.0;
                action.driveTrain.rawTurn = 0.0;
                state = MOVE_LIFT_UP1;
            }

            break;

        case WAIT_FOR_MOVE_LIFT_UP1:
            if(action.lift.doneState != action.lift.STALE) // message is available
            {
                if(action.lift.doneState == action.lift.SUCCESS)
                    state = RELEASE_TUBE1;
                else if(action.lift.doneState == action.lift.FAILURE)
                    state = IDLE; // TODO correct state
                else if(action.lift.doneState == action.lift.ABORTED)
                    state = IDLE; // TODO correct state
            }
            break;

        case WAIT_FOR_RELEASE_TUBE1:
            static int releaseCount1 = 0;

            // 0.75 second release
            if(++releaseCount1 % 37 == 0)
                state = MOVE_LIFT_DOWN1;
            break;

        case WAIT_FOR_MOVE_LIFT_DOWN1:
            if(action.lift.doneState != action.lift.STALE) // message is available
            {
                if(action.lift.doneState == action.lift.SUCCESS)
                    state = TURN_TO_GO_BACK;
                else if(action.lift.doneState == action.lift.FAILURE)
                    state = IDLE; // TODO correct state
                else if(action.lift.doneState == action.lift.ABORTED)
                    state = IDLE; // TODO correct state
            }
            break;

        case WAIT_FOR_TURN_TO_GO_BACK:
            if(action.positionTrain.done)
            {
                action.positionTrain.enabled = false;
                // should move distance is automatically reset
                action.positionTrain.pivotLeft = false;
                action.positionTrain.pivotRight = false;

                state = MOVE_BACK;
            }
            break;

        case WAIT_FOR_MOVE_BACK:
            if(action.positionTrain.done)
            {
                action.positionTrain.enabled = false;
                // should move distance is automatically reset

                if(numberOfTubes == 1)
                    state = IDLE;
                else
                    state = TURN_TO_PICKUP_RINGER;
            }
            break;

        case WAIT_FOR_TURN_TO_PICKUP_RINGER:
            if(action.positionTrain.done)
            {
                action.positionTrain.enabled = false;
                // should move distance is automatically reset

                state = DRIVE_TO_LINE;
            }
            break;

        case WAIT_FOR_DRIVE_TO_LINE:
            if(action.positionTrain.done)
            {
                action.positionTrain.enabled = false;
                // should move distance is automatically reset

                state = FOLLOW_LINE;
            }
            break;


        case WAIT_FOR_FOLLOW_LINE:
            AutomatedFollowLine();//Follow the line
            if(driveEncoders.GetRobotDist() > 12) //TODO FIGURE ME OUT!
                action.driveTrain.usingClosedLoop = false;
            if(driveEncoders.GetNormalizedForwardSpeed() < 0.2)
            {
                state = MOVE_LIFT_UP2;
                action.driveTrain.usingClosedLoop = true;
                action.driveTrain.rawForward = 0.0;
                action.driveTrain.rawTurn = 0.0;
            }
            break;

        case WAIT_FOR_MOVE_LIFT_UP2:
            if(action.lift.doneState != action.lift.STALE) // message is available
            {
                if(action.lift.doneState == action.lift.SUCCESS)
                    state = RELEASE_TUBE2;
                else if(action.lift.doneState == action.lift.FAILURE)
                    state = IDLE; // TODO correct state
                else if(action.lift.doneState == action.lift.ABORTED)
                    state = IDLE; // TODO correct state
            }
            break;

        case WAIT_FOR_RELEASE_TUBE2:
            static int releaseCount2 = 0;

            // 0.75 second release
            if(++releaseCount2 % 37 == 0)
                state = MOVE_LIFT_DOWN2;
            break;

        case WAIT_FOR_MOVE_LIFT_DOWN2:
            if(action.lift.doneState != action.lift.STALE) // message is available
            {
                if(action.lift.doneState == action.lift.SUCCESS)
                    state = MOVE_BACK2;
                else if(action.lift.doneState == action.lift.FAILURE)
                    state = IDLE; // TODO correct state
                else if(action.lift.doneState == action.lift.ABORTED)
                    state = IDLE; // TODO correct state
            }
            break;

    #define VICTORYDANCE IDLE
        case WAIT_FOR_MOVE_BACK2:
            if(action.positionTrain.done)
            {
                action.positionTrain.enabled = false;
                // should move distance is automatically reset

                state = VICTORYDANCE;// :P
            }
            break;

        case IDLE:
            //ne faire rien
            break;
        }

        */
}

void Brain::Side()
{
    // used for state machine
    static enum
    {
        DRIVE_FORWARD,
        RUN_INTO_PEG,
        MOVE_LIFT_UP,
        RELEASE_TUBE,
        WAIT_FOR_PEG,
        WAIT_FOR_DRIVE,
        WAIT_FOR_LIFT,
        WAIT_FOR_RELEASE,
        IDLE
    } state = DRIVE_FORWARD;

    switch(state)
    {
    case DRIVE_FORWARD:
        // switch to low gear and shift
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        action.positionTrain.enabled = true;
        action.positionTrain.shouldMoveDistance = true;

        // placeholder; distance to pegs in inches
        action.positionTrain.moveDistance = 36.0;

        state = WAIT_FOR_DRIVE;
        break;

    case RUN_INTO_PEG:
        action.driveTrain.usingClosedLoop = false;
        action.driveTrain.rawForward = 0.1;
        action.driveTrain.rawTurn = 0;
        state = WAIT_FOR_PEG;
        break;

    case MOVE_LIFT_UP:
        action.lift.givenCommand = true;
        action.lift.highRow = true; // should change based off position
        action.lift.preset = action.lift.HIGH_PEG;

        state = WAIT_FOR_LIFT;
        break;

    case RELEASE_TUBE:
        action.roller.rotateUpward = true;
        action.roller.state = action.roller.ROTATING;

        state = WAIT_FOR_RELEASE;
        break;

    case WAIT_FOR_DRIVE:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            // should move distance is automatically reset

            state = RUN_INTO_PEG;
        }
        break;

    case WAIT_FOR_PEG:
        if(DriveEncoders::GetInstance().GetNormalizedForwardSpeed() < 0.05)
        {
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.usingClosedLoop = true;
            state = MOVE_LIFT_UP;
        }
        break;

    case WAIT_FOR_LIFT:
        // wait until lift is done
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = RELEASE_TUBE;
            else if(action.lift.doneState == action.lift.FAILURE)
                state = IDLE; // TODO correct state
            else if(action.lift.doneState == action.lift.ABORTED)
                state = IDLE; // TODO correct state
        }
        break;

    case WAIT_FOR_RELEASE:
        static int releaseCount = 0;

        // two second release
        if(++releaseCount % 100 == 0)
            state = IDLE;
        break;

    case IDLE:
        break;
    }


}
