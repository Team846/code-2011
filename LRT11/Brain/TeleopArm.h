#include "Brain.h"

void Brain::TeleopArm()
{
    action.arm.manualMode = false;

    // assume command is given; set to false in last else
    // statement (see below)
    action.arm.givenCommand = true;

    if(inputs.ShouldGrabGamePiece())
        action.arm.presetTop = false;
    else
        action.arm.presetTop = true;

    //disable manual control of the arm
    //for now because not enough buttons
    /*if(inputs.ShouldMoveArmDown())
    {
        action.arm.manualMode = true;
        action.arm.manualUp = false;

        // set preset flag to false to avoid conflicts
        action.arm.presetTop = false;
    }
    else if(inputs.ShouldMoveArmUp())
    {
        action.arm.manualMode = true;
        action.arm.manualUp = true;

        // see comment above
        action.arm.presetTop = false;
    }
    else if(inputs.ShouldMoveArmTopPreset())
        action.arm.presetTop = true;
    else if(inputs.ShouldMoveArmBottomPreset())
        action.arm.presetTop = false;
    else
        // no command given
        action.arm.givenCommand = false;*/
}
