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

    // includes system of movements to release the ringer
    AutomatedRoutines();

    if(isFinale /* && inputs.ShouldDeployMinibot() */)
    {
        /* Minibot deployment code */
    }
}
