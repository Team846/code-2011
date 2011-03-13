#include "Brain.h"

void Brain::AutomatedRoutines()
{
    // used for state machine
    static enum
    {
        IDLE,
        ROTATING_RINGER,
        START_SPITTING,
        MOVE_LIFT_UP,
        MOVE_LIFT_DOWN,
        WAIT_FOR_LIFT,
        END_SPITTING,
        ABORT
    } state = IDLE;

    switch(state)
    {
    case IDLE:
        if(action.automation.releaseRinger)
            state = ROTATING_RINGER;
        break;

    case ROTATING_RINGER:
        static int rotateCount = 0;
        action.roller.rotateUpward = false;
        action.roller.state = action.roller.ROTATING;

        // one second of rotating
        if(++rotateCount % 50 == 0)
        {
            state = START_SPITTING;
            action.roller.state = action.roller.STOPPED;
        }
        break;

    case START_SPITTING:
        action.roller.state = action.roller.SPITTING;
        state = MOVE_LIFT_UP;
        break;

    case MOVE_LIFT_UP:
        // configuration for moving to a preset
        action.lift.givenCommand = true;
        action.lift.position = action.lift.HIGH_PEG;
        action.lift.highRow = false; // should change based off position

        state = WAIT_FOR_LIFT;
        break;

    case MOVE_LIFT_DOWN:
        // configuration for moving to a preset
        action.lift.givenCommand = true;
        action.lift.position = action.lift.STOWED;
        action.lift.highRow = false; // should change based off position

        state = WAIT_FOR_LIFT;
        break;

    case WAIT_FOR_LIFT:
        // wait until lift is done
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = END_SPITTING;
            else if(action.lift.doneState == action.lift.FAILURE)
                state = ABORT; // TODO correct state
            else if(action.lift.doneState == action.lift.ABORT)
                state = ABORT; // TODO correct state
        }
        break;

    case END_SPITTING:
        action.roller.state = action.roller.STOPPED;
        action.automation.releaseRinger = false;
        state = IDLE; // wait for next command
        break;

    case ABORT:
        // TODO abort
        break;
    }
}
