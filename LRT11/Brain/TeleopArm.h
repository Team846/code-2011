#include "Brain.h"

void Brain::TeleopArm()
{
    action.arm.manualDown = false;
    action.arm.manualUp = false;

    if(inputs.ShouldMoveArmDown())
    {
        action.arm.manualDown = true;
        action.arm.givenCommand = true;
    }
    else if(inputs.ShouldMoveArmUp())
    {
        action.arm.manualUp = true;
        action.arm.givenCommand = true;
    }
    else if(inputs.ShouldMoveArmTopPreset())
    {
        action.arm.presetTop = true;
        action.arm.givenCommand = true;
    }
    else if(inputs.ShouldMoveArmBottomPreset())
    {
        action.arm.presetBottom = true;
        action.arm.givenCommand = true;
    }
}
