#include "Brain.h"

void Brain::TeleopLift()
{
    action.lift.manualMode = false;

    // assume command is given; set to false in last else
    // statement (see below)
    action.lift.givenCommand = true;

    // determine lift movement with else ifs for interlocks
    if(inputs.ShouldMoveLiftLow())
        action.lift.position = action.lift.LOW_PEG;
    else if(inputs.ShouldMoveLiftMedium())
        action.lift.position = action.lift.MED_PEG;
    else if(inputs.ShouldMoveLiftHigh())
        action.lift.position = action.lift.HIGH_PEG;
    else if(inputs.ShouldManuallyPowerLift())
    {
        action.lift.manualMode = true;
        action.lift.power = inputs.GetLiftPower();
    }
    else
        // no command given
        action.lift.givenCommand = false;
}
