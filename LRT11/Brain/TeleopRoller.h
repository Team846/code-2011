#include "Brain.h"

void Brain::TeleopRoller()
{
    // spitting the ringer out
    if(inputs.ShouldRollerSpit())
        action.roller.state = action.roller.SPITTING;
    // sucking the ringer in
    else if(inputs.ShouldRollerSuck())
        action.roller.state = action.roller.SUCKING;
    // rotating the ringer upward/downward
    else if(inputs.ShouldRollerRotate())
    {
        action.roller.rotateUpward = inputs.GetOperatorThrottle();
        action.roller.state = action.roller.ROTATING;
    }
    // spitting the ringer (automated routine)
    else if(inputs.ShouldRollerBeAutomated())
    {
        static enum
        {
            IDLE,
            ROTATING,
            SPITTING
        } state = IDLE;

        // begin, or set the state, only when the button is just pressed
        if(inputs.ShouldRollerCommenceAutomation())
            state = ROTATING;

        switch(state)
        {
        case IDLE:
            break;

        case ROTATING:
            static int rotateCycle = 0;

            action.roller.rotateUpward = false;
            action.roller.state = action.roller.ROTATING;

            // rotate for half a second
            if(++rotateCycle % 25 == 0)
                state = ROTATING;
            break;

        case SPITTING:
            static int spitCycle = 0;
            action.roller.state = action.roller.SPITTING;

            // spit for one second
            if(++spitCycle % 100 == 0)
                state = IDLE;
            break;
        }
    }
    else
        action.roller.state = action.roller.STOPPED;

    /*
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
        else if(action.arm.givenCommand && !action.arm.manualMode)
        {
            if(action.arm.presetTop)
                state = ARM_GOING_TO_TOP;
            else
                state = ARM_GOING_TO_BOTTOM;
        }
        break;

    case ARM_GOING_TO_BOTTOM:
        // wait until arm is done
        if(action.arm.doneState != action.arm.STALE) // message is available
        {
            if(action.arm.doneState == action.arm.SUCCESS)
            {
                action.roller.state = action.roller.SUCKING;
                state = IDLE;
            }
            else if(action.arm.doneState == action.arm.FAILURE)
                state = IDLE; // TODO correct state
            else if(action.arm.doneState == action.arm.ABORT)
                state = IDLE; // TODO correct state
        }
        break;

    case ARM_GOING_TO_TOP:
        // see comments above
        if(action.arm.doneState != action.arm.STALE)
        {
            if(action.arm.doneState == action.arm.SUCCESS)
                // ringer needs to be rotated to vertical position
                state = SQUARING_RINGER_WITH_LIFT;
            else if(action.arm.doneState == action.arm.FAILURE)
                // maneuver not successful; return to waiting state
                state = IDLE; // TODO correct state
            else if(action.arm.doneState == action.arm.ABORT)
                state = IDLE; // TODO correct state
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
    */
}
