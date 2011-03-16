#include "Brain.h"

void Brain::TeleopRoller()
{
    if(inputs.ShouldRollerSpit())
        action.roller.state = action.roller.SPITTING;
    // grab game piece is a driver-controlled button
    else if(inputs.ShouldGrabGamePiece() || inputs.ShouldRollerSuck())
        action.roller.state = action.roller.SUCKING;
    // spitting the ringer out (automated)
    else if(inputs.ShouldRollerBeAutomated())
    {
        static enum
        {
            ROTATING,
            SPITTING
        } state = ROTATING;

        static int rotateCycle = 0, spitCycle = 0;

        // begin, or set the state, only when the button is just pressed
        if(inputs.ShouldRollerCommenceAutomation())
        {
            state = ROTATING;
            rotateCycle = 0;
            spitCycle = 0;
        }

        switch(state)
        {
        case ROTATING:
            action.roller.rotateUpward = false;
            action.roller.state = action.roller.ROTATING;

            // rotate for half a second
            if(++rotateCycle % 5 == 0)
                state = SPITTING;
            break;

        case SPITTING:
            action.roller.state = action.roller.SPITTING;

            // stay in spitting mode until operator stops pushing button
            break;
        }
    }
    else if(inputs.ShouldRollerRotateUp())
    {
        action.roller.rotateUpward = true;
        action.roller.state = action.roller.ROTATING;
    }
    else if(inputs.ShouldRollerRotateDown())
    {
        action.roller.rotateUpward = false;
        action.roller.state = action.roller.ROTATING;
    }
    else
        action.roller.state = action.roller.STOPPED;
}
