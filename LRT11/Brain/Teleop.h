#include "Brain.h"
#include "..\ActionData.h"

void Brain::Teleop()
{
    action.driveTrain.rawForward = inputs.GetForward();
    action.driveTrain.rawTurn    = inputs.GetTurn();

    action.driveTrain.brakeLeft  = inputs.GetBrakeLeft();
    action.driveTrain.brakeRight = inputs.GetBrakeRight();

    TeleopLift();

    if(inputs.GetShiftLow())
    {
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;
    }
    else if(inputs.GetShiftHigh())
    {
        action.shifter.gear = action.shifter.HIGH_GEAR;
        action.shifter.force = true;
    }

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

    if(isFinale /* && inputs.ShouldDeployMinibot() */)
    {
        /* Minibot deployment code */
    }
}
