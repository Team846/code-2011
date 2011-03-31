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
    	action.arm.action = action.arm.PRESETBOTTOM;
    else if(inputs.ShouldMoveArmUp())
    	action.arm.action = action.arm.PRESETBOTTOM;

    // driver wants the arm down and the roller to rotate
    else if(inputs.ShouldGrabGamePiece())
    {
        //don't force the arm to keep going down if has reached that point
        if(armState != ARM_DOWN)
            action.arm.action = action.arm.PRESETBOTTOM;
    }
    //default case
    else
    	action.arm.action = action.arm.PRESETTOP;

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
