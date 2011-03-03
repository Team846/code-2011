#include "Brain.h"

void Brain::TeleopArm()
{
    action.arm.manualDown = false;
    action.arm.manualUp = false;

    // assume command is given; set to false in last else
    // statement (see below)
    action.arm.givenCommand = true;

    // determine arm movement with else ifs for interlocks
    if(inputs.ShouldMoveArmDown())
    {
        action.arm.manualDown = true;

        // could interrupt preset mode; set flags to false to
        // prevent conflicts
        action.arm.presetTop = false;
        action.arm.presetBottom = false;
    }
    else if(inputs.ShouldMoveArmUp())
    {
        action.arm.manualUp = true;

        // see comment above
        action.arm.presetTop = false;
        action.arm.presetBottom = false;
    }
    else if(inputs.ShouldMoveArmTopPreset())
    {
        // could interrupt a different preset maneuver; set both
        // preset flags to prevent issues
        action.arm.presetTop = true;
        action.arm.presetBottom = false;
    }
    else if(inputs.ShouldMoveArmBottomPreset())
    {
        // see comment above
        action.arm.presetBottom = true;
        action.arm.presetTop = false;
    }
    else
        // no command given
        action.arm.givenCommand = false;
}
