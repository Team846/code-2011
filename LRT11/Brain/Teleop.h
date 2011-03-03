#include "Brain.h"
#include "..\ActionData.h"

void Brain::Teleop()
{
    TeleopDriveTrain();
    TeleopShifter();

    TeleopLift();
    TeleopArm();

    if(isFinale /* && inputs.ShouldDeployMinibot() */)
    {
        /* Minibot deployment code */
    }
}
