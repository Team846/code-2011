#include "Brain.h"

void Brain::TeleopLift()
{
    action.lift.manualMode = false;

    if(inputs.ShouldMoveLiftLow())
    {
        action.lift.position = action.lift.LOW_PEG;
        action.lift.givenCommand = true;
    }
    else if(inputs.ShouldMoveLiftMedium())
    {
        action.lift.position = action.lift.MED_PEG;
        action.lift.givenCommand = true;
    }
    else if(inputs.ShouldMoveLiftHigh())
    {
        action.lift.position = action.lift.HIGH_PEG;
        action.lift.givenCommand = true;
    }
    else if(inputs.ShouldManuallyPowerLift())
    {
        action.lift.manualMode = true;
        action.lift.power = inputs.GetLiftPower();
        action.lift.givenCommand = true;
    }
}
