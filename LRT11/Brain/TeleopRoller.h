#include "Brain.h"

enum
{
    WAITING,
    ARM_GOING_TO_BOTTOM,
    ARM_GOING_TO_TOP,
    SQUARING_RINGER_WITH_LIFT,
    STOPPING
} rollerState = WAITING;

void Brain::TeleopRoller()
{
    switch(rollerState)
    {
    case WAITING:
        if(action.arm.givenCommand)
        {
            if(action.arm.presetTop)
                rollerState = ARM_GOING_TO_TOP;
            else if(action.arm.presetBottom)
                rollerState = ARM_GOING_TO_BOTTOM;
        }
        break;

    case ARM_GOING_TO_BOTTOM:
        // if the preset flag is no longer set, the maneuver has
        // completed, but was not necessarily successful
        if(!action.arm.presetBottom)
        {
        	// action.arm.done represents success
            if(action.arm.done)
                action.roller.state = action.roller.SUCKING;

            // wait for next arm command
            rollerState = WAITING;
        }
        break;

    case ARM_GOING_TO_TOP:
        // see comment from ARM_GOING_TO_BOTTOM
        if(!action.arm.presetTop)
        {
            if(action.arm.done)
                // ringer needs to be rotated to vertical position
                rollerState = SQUARING_RINGER_WITH_LIFT;
            else
                // maneuver not successful; return to waiting state
                rollerState = WAITING;
        }
        break;

    case SQUARING_RINGER_WITH_LIFT:
        static int count = 0;

        // square ringer with the lift
        action.roller.state = action.roller.ROTATING;
        action.roller.rotateUpward = true;

        // finish squaring up after 1 second (50 cycles)
        if(++count % 50 == 0)
            rollerState = STOPPING;

        // might have to back off
        break;

    case STOPPING:
        action.roller.state = action.roller.STOPPED;
        rollerState = WAITING;
        break;
    }
}
