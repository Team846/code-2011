#include "Brain.h"

void Brain::TeleopRoller()
{
    if(inputs.ShouldRollerSpit())
        action.roller.state = action.roller.SPITTING;
    // grab game piece is a driver-controlled button
    else if(inputs.ShouldGrabGamePiece() || inputs.ShouldRollerSuck())
        action.roller.state = action.roller.SUCKING;
    // spitting the ringer out (automated)
    else if(inputs.ShouldRollerBeAutomated() || action.roller.automated)
    {
        static enum
        {
//            ROTATING,
            SPITTING,
            MOVING_LIFT_DOWN,
            STOPPING
        } state = SPITTING;

        static int timer = 0;

        // begin, or set the state, only when the button is just pressed
        if(inputs.ShouldRollerCommenceAutomation() || action.roller.commenceAutomation)
        {
            timer = 0;
            state = SPITTING;
        }

        switch(state)
        {
        case SPITTING:
            action.roller.state = action.roller.SPITTING;
            state = MOVING_LIFT_DOWN;

            break;

        case MOVING_LIFT_DOWN:
            action.lift.givenCommand = true;
            action.lift.manualMode = true;
            action.lift.power = -0.4;

            // keep going until the button is released
            // one and a half seconds
//            if(++timer % 50 == 0)
//            {
//                state = STOPPING;
//                timer = 0; // reset timer
//            }
            break;

        case STOPPING:
            action.roller.state = action.roller.STOPPED;
            break;
        }
    }
    // rotate roller upward
    else if(inputs.ShouldRollerRotateUp())
    {
        action.roller.rotateUpward = true;
        action.roller.state = action.roller.ROTATING;
    }
    // rotate roller downaward
    else if(inputs.ShouldRollerRotateDown())
    {
        action.roller.rotateUpward = false;
        action.roller.state = action.roller.ROTATING;
    }
    else
        action.roller.state = action.roller.STOPPED;
}
