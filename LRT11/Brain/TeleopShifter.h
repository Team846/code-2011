#include "Brain.h"

void Brain::TeleopShifter()
{
	// assume force shift; set to false in last else
	// statement (see below)
	action.shifter.force = true;
	
    if(inputs.ShouldShiftLow())
        action.shifter.gear = action.shifter.LOW_GEAR;
    else if(inputs.ShouldShiftHigh())
        action.shifter.gear = action.shifter.HIGH_GEAR;
    else
    	// no force shift
    	action.shifter.force = false;
}
