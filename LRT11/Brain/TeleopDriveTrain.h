#include "Brain.h"

void Brain::TeleopDriveTrain()
{
#ifdef VIRTUAL
    // can't use closed loop with a virtual massless system
    action.driveTrain.usingClosedLoop = false;
#endif

//    action.shifter.gear = action.shifter.LOW_GEAR;
//    action.positionTrain.enabled = true;
//
//    if(!hasMoved)
//    {
//        AsynchronousPrinter::Printf("Has Moved\n");
//        action.positionTrain.shouldMoveDistance = true;
//        action.positionTrain.moveDistance = 24.0; // inches
//        hasMoved = true;
//    }

//    action.driveTrain.rawForward = 0.1;
//    action.driveTrain.rawTurn = 0.0;
//    return;

    action.driveTrain.rawForward = inputs.GetForward();
    action.driveTrain.rawTurn    = inputs.GetTurn();

    action.driveTrain.brakeLeft  = inputs.ShouldBrakeLeft();
    action.driveTrain.brakeRight = inputs.ShouldBrakeRight();

    action.driveTrain.thirdGear  = inputs.ShouldShiftThird();

}
