#include "Brain.h"

void Brain::TeleopShifter()
{
    // used to only force shift the first time
    static bool forceShiftedLow = false;
    static bool forceShiftedHigh = false;

    //must set this way since is static
    static int lastGear = ActionData::GetInstance().shifter.gear;

    // assume no force shift
    action.shifter.force = false;

    // always low gear unless shift high button is pushed down
    if(inputs.ShouldShiftHigh())
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
    else
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

    //we commenced a shift
    if(lastGear != action.shifter.gear)
        action.driveTrain.mode = action.driveTrain.SYNCHRONIZING;

    lastGear = action.shifter.gear;
}
