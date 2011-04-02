#include "Brain.h"

void Brain::TeleopArm()
{
    // default to the arm at the top state
//    action.arm.state = action.arm.PRESET_TOP;
    action.arm.state = action.arm.PRESET_TOP;

    if(inputs.ShouldMoveArmDown())
        action.arm.state = action.arm.MANUAL_DOWN;
    else if(inputs.ShouldMoveArmUp())
        action.arm.state = action.arm.MANUAL_UP;
    // driver wants the arm down and the roller to rotate
    else if(inputs.ShouldGrabGamePiece())
        action.arm.state = action.arm.PRESET_BOTTOM;

    if(wasDisabled)
        // must set to IDLE to register state change
        action.arm.state = action.arm.IDLE;

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
