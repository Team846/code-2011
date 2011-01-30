#include "Brain.h"
#include "..\ActionData.h"

void Brain::Disabled()
{
    action.driveTrain.rawForward = 0;
    action.driveTrain.rawTurn    = 0;
}
