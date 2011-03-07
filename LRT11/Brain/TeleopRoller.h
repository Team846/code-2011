#include "Brain.h"

void Brain::TeleopRoller()
{
    // used for state machine
    static enum
    {
        IDLE,
        ARM_GOING_TO_BOTTOM,
        ARM_GOING_TO_TOP,
        SQUARING_RINGER_WITH_LIFT,
        STOPPING,
        SPITTING
    } state = IDLE;

    switch(state)
    {
    case IDLE:
        // spitting overrides other modes
        if(inputs.ShouldSpitRoller())
            state = SPITTING;
        else if(action.arm.givenCommand)
        {
            if(action.arm.presetTop)
                state = ARM_GOING_TO_TOP;
            else if(action.arm.presetBottom)
                state = ARM_GOING_TO_BOTTOM;
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
            state = IDLE;
        }
        break;

    case ARM_GOING_TO_TOP:
        // see comment from ARM_GOING_TO_BOTTOM
        if(!action.arm.presetTop)
        {
            if(action.arm.done)
                // ringer needs to be rotated to vertical position
                state = SQUARING_RINGER_WITH_LIFT;
            else
                // maneuver not successful; return to waiting state
                state = IDLE;
        }
        break;

    case SQUARING_RINGER_WITH_LIFT:
        static int squaringCycles = 0;

        // square ringer with the lift
        action.roller.state = action.roller.ROTATING;
        action.roller.rotateUpward = true;

        // finish squaring up after 1 second (50 cycles)
        if(++squaringCycles % 50 == 0)
            state = STOPPING;

        // TODO might have to back off
        break;

    case STOPPING:
        action.roller.state = action.roller.STOPPED;
        state = IDLE;
        break;

    case SPITTING:
        action.roller.state = action.roller.SPITTING;

        // if button is released, stop the roller
        if(!inputs.ShouldSpitRoller())
            state = STOPPING;
        break;
    }
}
