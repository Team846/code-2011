#include "Brain.h"
#include "..\ActionData.h"

void Brain::Teleop()
{
    TeleopDriveTrain();
    TeleopShifter();

    TeleopLift();
    TeleopArm();
    TeleopRoller();

    TeleopMinibot();

    // includes automated routines such as line sensing and
    // dead-reckoning autonomous mode
//    AutomatedRoutines();

    if(inputs.ShouldAbort())
        action.master.abort = true;
    else
        action.master.abort = false;
}
