#include "Brain.h"

void Brain::TeleopShifter()
{
    if(inputs.ShouldShiftLow())
    {
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;
    }
    else if(inputs.ShouldShiftHigh())
    {
        action.shifter.gear = action.shifter.HIGH_GEAR;
        action.shifter.force = true;
    }
}
