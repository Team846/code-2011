#include "Brain.h"

void Brain::TeleopDriveTrain()
{
    action.driveTrain.rawForward = inputs.GetForward();
    action.driveTrain.rawTurn    = inputs.GetTurn();

    action.driveTrain.brakeLeft  = inputs.ShouldBrakeLeft();
    action.driveTrain.brakeRight = inputs.ShouldBrakeRight();
}
