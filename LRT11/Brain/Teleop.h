#include "Brain.h"
#include "..\ActionData.h"

void Brain::Teleop()
{
    action.driveTrain.rawForward = inputs.GetForward();
    action.driveTrain.rawTurn    = inputs.GetTurn();

    action.driveTrain.brakeLeft  = inputs.GetBrakeLeft();
    action.driveTrain.brakeRight = inputs.GetBrakeRight();

    TeleopLift();

//    static bool shouldMoveDistance = false;
//    action.positionTrain.shouldMoveDistance = false;
//    action.positionTrain.shouldTurnAngle    = shouldMoveDistance;
//    shouldMoveDistance = false;
//
//    if(previousState == DISABLED)
//        shouldMoveDistance = true;
//
//    action.positionTrain.pivotLeft = false;
//    action.positionTrain.pivotRight = false;
//    action.positionTrain.turnAngle = 90;

//    action.encoderData.shouldCollect = inputs.ShouldCollectEncoderData();
    action.gearbox.gear = action.gearbox.LOW_GEAR;

    if(isFinale /* && inputs.ShouldDeployMinibot() */)
    {
        /* Minibot deployment code */
    }
}
