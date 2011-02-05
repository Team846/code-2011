#include "Brain.h"
#include "..\ActionData.h"

void Brain::Teleop()
{
    action.driveTrain.rawForward = inputs.GetForward();
    action.driveTrain.rawTurn    = inputs.GetTurn();

    action.driveTrain.brakeLeft  = inputs.GetBrakeLeft();
    action.driveTrain.brakeRight = inputs.GetBrakeRight();

    action.encoderData.shouldCollect = inputs.ShouldCollectEncoderData();

    if(isFinale /* && inputs.DeployMinibot() */)
    {
        /* Minibot deployment code */
    }
}
