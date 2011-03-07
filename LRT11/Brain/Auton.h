#include "Brain.h"

void Brain::Auton()
{
    // used for state machine
    static enum
    {
        DRIVE_FORWARD,
        MOVE_LIFT,
        RELEASE_TUBE,
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

    case MOVE_LIFT:
        action.lift.givenCommand = true;
        action.lift.highRow = false; // should change based off position
        action.lift.position = action.lift.HIGH_PEG;

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

            state = MOVE_LIFT;
        }
        break;

    case WAIT_FOR_LIFT:
        if(action.lift.done)
        {
            // disable the lift
            action.lift.givenCommand = false;

            state = RELEASE_TUBE;
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

    // includes system of movements to release the ringer
    AutomatedRoutines();
}
