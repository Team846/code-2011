#include "Brain.h"
#include "..\ActionData\DriveAction.h"

void Brain::Disabled()
{
    // reset action data values
    action.driveTrain->rate.rawForward = 0;
    action.driveTrain->rate.rawTurn    = 0;

    action.driveTrain->position.givenCommand = false;
    action.driveTrain->distance.givenCommand = false;

    hasMoved = false;
    wasDisabledLastCycle = true;

    static int cycleCount = 0;
    if(++cycleCount % 50 == 0) // check every second
        Config::GetInstance().CheckForFileUpdates();
}
