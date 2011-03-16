#include "Brain.h"

void Brain::TeleopArm()
{
    action.arm.manualMode = false;

    // assume command is not given
    action.arm.givenCommand = false;

    static enum
    {
        ARM_UP,
        ARM_DOWN,
        ARM_STALE
    } armState = ARM_UP;

    if(inputs.ShouldMoveArmDown())
    {
        action.arm.givenCommand = true;
        action.arm.manualMode = true;
        action.arm.manualUp = false;

        // set preset flag to false to avoid conflicts
        action.arm.presetTop = false;
        armState = ARM_STALE; // not sure where the arm is
    }
    else if(inputs.ShouldMoveArmUp())
    {
        action.arm.givenCommand = true;
        action.arm.manualMode = true;
        action.arm.manualUp = true;

        // see comment above
        action.arm.presetTop = false;
        armState = ARM_STALE; // not sure where the arm is
    }
    // driver wants the arm down and the roller to rotate
    else if(inputs.ShouldGrabGamePiece())
    {
        // only set the preset once
        if(armState != ARM_DOWN)
        {
            action.arm.givenCommand = true;

            // bottom preset
            action.arm.presetTop = false;
            armState = ARM_DOWN;
        }
    }
    else
    {
        // only set the preset once
        if(armState != ARM_UP)
        {
            action.arm.givenCommand = true;

            // top preset
            action.arm.presetTop = true;
            armState = ARM_UP;
        }
    }

    // if the arm failed or aborted at some point, set the
    // state to stale
    if(action.arm.doneState == action.arm.FAILURE || action.arm.doneState == action.arm.ABORTED)
        armState = ARM_STALE; // not sure where the arm is

    // operator preset control currently disabled, as driver
    // has the ability to move the arm up and down
//    else if(inputs.ShouldMoveArmTopPreset())
//        action.arm.presetTop = true;
//    else if(inputs.ShouldMoveArmBottomPreset())
//        action.arm.presetTop = false;
//    else
//        // no command given
//        action.arm.givenCommand = false;
}
