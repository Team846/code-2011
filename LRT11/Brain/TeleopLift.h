#include "Brain.h"

void Brain::TeleopLift()
{
    action.demoLift.forward = inputs.GetOperatorForwardScaled30();
}
