#include "Brain.h"

void Brain::TeleopDriveTrain()
{
#ifdef VIRTUAL
    // can't use closed loop with a virtual massless system
    action.driveTrain.usingClosedLoop = false;
#endif

    // used for calibration
    if(ds.GetDigitalIn(8))
    {
        if(wasDisabled)
        {
            action.driveTrain.mode = action.driveTrain.POSITION;
            action.driveTrain.position.givenCommand = true;

            action.driveTrain.position.shouldMoveDistance = true;
            action.driveTrain.position.shouldTurnAngle = false;

            action.driveTrain.position.distanceSetPoint = 10.0 * 12; // 10 feet forward
            action.driveTrain.position.turnSetPoint = 0.0;

            action.driveTrain.position.maxFwdSpeed = 0.3;
            action.driveTrain.position.maxTurnSpeed = 1.0;
        }

//        if(action.driveTrain.distance.done)
//        {
//            action.driveTrain.mode = action.driveTrain.RATE;
//            action.driveTrain.rate.rawForward = 0.0;
//            action.driveTrain.rate.rawTurn = 0.0;
//        }
    }
    else
    {
        action.driveTrain.mode = action.driveTrain.RATE;
        action.driveTrain.rate.rawForward = inputs.GetForward();
        action.driveTrain.rate.rawTurn    = inputs.GetTurn();

        action.driveTrain.rate.brakeLeft  = inputs.ShouldBrakeLeft();
        action.driveTrain.rate.brakeRight = inputs.ShouldBrakeRight();

        action.driveTrain.rate.thirdGear  = inputs.ShouldShiftThird();
    }
}
