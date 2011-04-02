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

    AutomatedRoutines();

    if(inputs.ShouldAbort())
        action.master.abort = true;
    else
        action.master.abort = false;
}
