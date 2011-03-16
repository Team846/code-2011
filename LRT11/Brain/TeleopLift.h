#include "Brain.h"

void Brain::TeleopLift()
{
    action.lift.manualMode = false;

    // assume command is given; set to false in last else
    // statement (see below)
    action.lift.givenCommand = true;

    // determine lift movement with else ifs for interlocks
    if(inputs.ShouldAbort())
    {
        action.lift.position = action.lift.STOWED;
        action.lift.highRow = inputs.IsHighRow();
        action.lift.manualMode = false;
    }
    else if(inputs.ShouldMoveLiftLow())
    {
        action.lift.position = action.lift.LOW_PEG;
        action.lift.highRow = inputs.IsHighRow();
        action.lift.manualMode = false;
    }
    else if(inputs.ShouldMoveLiftMedium())
    {
        action.lift.position = action.lift.MED_PEG;
        action.lift.highRow = inputs.IsHighRow();
        action.lift.manualMode = false;
    }
    else if(inputs.ShouldMoveLiftHigh())
    {
        action.lift.position = action.lift.HIGH_PEG;
        action.lift.highRow = inputs.IsHighRow();
        action.lift.manualMode = false;
    }
    else if(inputs.ShouldPulseLiftUp())
    {
        action.lift.manualMode = true;
        action.lift.power = 0.2;//TODO Check me
    }
    else if(inputs.ShouldPulseLiftUp())
    {
        action.lift.manualMode = true;
        action.lift.power = -0.2;//TODO Check me
    }
//    else if(inputs.ShouldManuallyPowerLift())
//    {
//        action.lift.manualMode = true;
//        action.lift.power = inputs.GetLiftPower();
//    }
    else
        // no command given
        action.lift.givenCommand = false;
}
