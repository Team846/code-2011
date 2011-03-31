#include "Brain.h"

void Brain::Disabled()
{
    // reset action data values
    action.driveTrain.rate.rawForward = 0;
    action.driveTrain.rate.rawTurn    = 0;

    action.driveTrain.position.givenCommand = false;
    action.driveTrain.distance.givenCommand = false;

    action.lift.givenCommand = false;

    // default to locked aligner
    action.deployer.shouldAlignerRelease = false;

    hasMoved = false;
    wasDisabled = true;
}
