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
            MOVING_LIFT_DOWN,
        } state = MOVING_LIFT_DOWN;

        static int timer = 0;

        // begin, or set the state, only when the button is just pressed
        if(inputs.ShouldRollerCommenceAutomation() || action.roller.commenceAutomation)
        {
            timer = 0;
            state = MOVING_LIFT_DOWN;
        }

        switch(state)
        {
        case MOVING_LIFT_DOWN:
            action.lift.givenCommand = true;
            action.lift.manualMode = true;
            action.lift.power = -0.4;

            // keep moving down for one tenth of a second
            if(++timer > 20)
                action.roller.state = action.roller.SPITTING;
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
