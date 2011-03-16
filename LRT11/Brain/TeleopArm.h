#include "Brain.h"

void Brain::TeleopArm()
{
    action.arm.manualMode = false;

    // assume command is not given
    action.arm.givenCommand = false;
    static bool isArmDown = true;

    // driver wants the arm down and the roller to rotate
    if(inputs.ShouldGrabGamePiece())
    {
        if(!isArmDown)
        {
            // only set the down preset once
            action.arm.givenCommand = true;
            action.arm.presetTop = false;
            isArmDown = true;
        }
    }
    else
    {
        if(isArmDown)
        {
            // only set the preset once
            action.arm.givenCommand = true;
            action.arm.presetTop = true;
            isArmDown = false;
        }
    }

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
