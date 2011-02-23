#include "Brain.h"
#include "..\ActionData.h"

void Brain::Teleop()
{
    TeleopDriveTrain();

    TeleopLift();
    TeleopArm();

    TeleopShifter();

    if(isFinale /* && inputs.ShouldDeployMinibot() */)
    {
        /* Minibot deployment code */
    }
}
