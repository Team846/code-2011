#include "Brain.h"

enum
{
    DRIVE_FORWARD,
    MOVE_LIFT,
    RELEASE_TUBE,
    WAIT_FOR_DRIVE,
    WAIT_FOR_LIFT,
    WAIT_FOR_RELEASE,
    IDLE
} autonState = DRIVE_FORWARD;

void Brain::Auton()
{
    switch(autonState)
    {
    case DRIVE_FORWARD:
        action.positionTrain.enabled = true;
        action.positionTrain.shouldMoveDistance = true;

        // placeholder; distance to pegs in inches
        action.positionTrain.moveDistance = 36.0;

        autonState = WAIT_FOR_DRIVE;
        break;

    case MOVE_LIFT:
        action.lift.givenCommand = true;
        action.lift.highRow = false; // should change based off position
        action.lift.position = action.lift.HIGH_PEG;

        autonState = WAIT_FOR_LIFT;
        break;

    case RELEASE_TUBE:
    	action.roller.state = action.roller.SPITTING;

        autonState = WAIT_FOR_RELEASE;
        break;

    case WAIT_FOR_DRIVE:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            // should move distance is automatically reset

            autonState = MOVE_LIFT;
        }
        break;

    case WAIT_FOR_LIFT:
        if(action.lift.done)
        {
        	// disable the lift
            action.lift.givenCommand = false;

            autonState = RELEASE_TUBE;
        }
        break;

    case WAIT_FOR_RELEASE:
    	static int releaseCount = 0;
    	
    	// two second release
    	if(++releaseCount % 100 == 0)
    		autonState = IDLE;
        break;
    	
    case IDLE:
    	break;
    }
}
