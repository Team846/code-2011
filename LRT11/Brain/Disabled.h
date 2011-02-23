#include "Brain.h"

void Brain::Disabled()
{
    action.driveTrain.rawForward = 0;
    action.driveTrain.rawTurn    = 0;

    action.lift.givenCommand = false;
    action.arm.givenCommand  = false;
}
