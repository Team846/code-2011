#include "Brain.h"

enum
{
    DRIVE_FORWARD,
    MOVE_LIFT,
    RELEASE_TUBE,
    WAIT_FOR_DRIVE,
    WAIT_FOR_LIFT,
    WAIT_FOR_RELEASE
} state = DRIVE_FORWARD;

void Brain::Auton()
{
    switch(state)
    {
    case DRIVE_FORWARD:
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
        // TODO make release routine

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
            action.lift.givenCommand = false;

            state = RELEASE_TUBE;
        }
        break;

    case WAIT_FOR_RELEASE:
        // TODO check if relase is done
        // no state change; done with routine
        break;
    }
}
