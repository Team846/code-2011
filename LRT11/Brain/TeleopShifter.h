#include "Brain.h"

void Brain::TeleopShifter()
{
    // used to only force shift the first time
    static bool forceShiftedLow = false;
    static bool forceShiftedHigh = false;

    // assume no force shift
    action.shifter.force = false;

    // always high gear unless shift low button is pushed down
    if(inputs.ShouldShiftLow())
    {
        // only force shift the first time
        if(!forceShiftedLow)
        {
            action.shifter.gear = action.shifter.LOW_GEAR;
            action.shifter.force = true;
            forceShiftedLow = true;
        }

        // shifted to low gear; set high gear flag to false
        forceShiftedHigh = false;
    }
//    else if(inputs.ShouldShiftHigh())
//        action.shifter.gear = action.shifter.HIGH_GEAR;
    else
    {
        // only force shift the first time
        if(!forceShiftedHigh)
        {
            action.shifter.gear = action.shifter.HIGH_GEAR;
            action.shifter.force = true;
            forceShiftedHigh = true;
        }

        // shifted to high gear; set low gear flag to false
        forceShiftedLow = false;
    }
}
