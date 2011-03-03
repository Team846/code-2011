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
        action.arm.manualDown = true;
    else if(inputs.ShouldMoveArmUp())
        action.arm.manualUp = true;
    else if(inputs.ShouldMoveArmTopPreset())
        action.arm.presetTop = true;
    else if(inputs.ShouldMoveArmBottomPreset())
        action.arm.presetBottom = true;
    else
    	// no command given
    	action.arm.givenCommand = false;
}
