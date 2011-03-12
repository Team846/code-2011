#include "Brain.h"

void Brain::Auton()
{
    // used for state machine
    static enum
    {
        DRIVE_FORWARD,
        RUN_INTO_PEG,
        MOVE_LIFT,
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
    	action.driveTrain.closedLoop = false;
    	action.driveTrain.rawForward = 0.1;
    	action.driveTrain.rawTurn = 0;
    	state = WAIT_FOR_PEG;
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

            state = RUN_INTO_PEG;
        }
        break;
        
    case WAIT_FOR_PEG:
    	if (DriveEncoders::GetInstance().GetNormalizedForwardSpeed() < 0.05)
    	{
    		action.driveTrain.rawForward = 0.0;
    		action.driveTrain.closedLoop = true;
	    	state = MOVE_LIFT;
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
            else if(action.lift.doneState == action.lift.ABORT)
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

    // includes system of movements to release the ringer
    AutomatedRoutines();
}
