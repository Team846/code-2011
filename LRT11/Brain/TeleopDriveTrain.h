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

    static int wait = 0;

    if(wait != 0)
    {
        wait--;
        return;
    }

    static enum
    {
        SET_COMMAND,
        DRIVE_FORWARD,
        STALL_DETECTION,
        SET_SECOND_COMMAND,
        DRIVE_BACK,
        IDLE
    } state = SET_COMMAND;

    static int timer = 0;

    AsynchronousPrinter::Printf("state:%d\n", (int) state);
    switch(state)
    {
    case SET_COMMAND:
        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = true;
        action.driveTrain.position.distanceSetPoint = 6.0 * 12; // 6 feet
        action.driveTrain.position.maxFwdSpeed = 0.3;

        timer = 0;
        state = DRIVE_FORWARD;
        break;

    case DRIVE_FORWARD:
        if(++timer > 50 * 5)
        {
            wait = 50;
            timer = 0;
            state = STALL_DETECTION;
        }
        break;

    case STALL_DETECTION:
        action.driveTrain.mode = action.driveTrain.SPEED;
        action.driveTrain.rate.usingClosedLoop = false;

        action.driveTrain.rate.rawForward = 0.3;
        action.driveTrain.rate.rawTurn = 0.0;

        if(++timer > 25)
        {
            if(driveEncoders.GetNormalizedForwardSpeed()
                    < 0.1)
            {
                wait = 50;
                action.driveTrain.rate.rawForward = 0.0;
                action.driveTrain.rate.rawTurn = 0.0;
                state = SET_SECOND_COMMAND;
            }
        }
        break;

    case SET_SECOND_COMMAND:
        action.driveTrain.rate.usingClosedLoop = true;
        action.driveTrain.rate.rawForward = 0.0;
        action.driveTrain.rate.rawTurn = 0.0;

        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = true;
        action.driveTrain.position.distanceSetPoint = -6.0; // 6 inches back

        action.driveTrain.position.maxFwdSpeed = 0.15;

        timer = 0;
        state = DRIVE_BACK;
        break;

    case DRIVE_BACK:
        // wait one second for driving to finish
        if(++timer > 50)
            state = IDLE;
        break;

    case IDLE:
        break;
    }
//
//
//    static int wait = -1;
//
//    if(wait > 0)
//    {
//      wait--;
//    }
//
//    static int cycleCount = 0;
//    static bool first = true;
//    if (first || cycleCount == -1)
//    {
//      action.driveTrain.resetFwd  = true;
//      action.driveTrain.resetTurn = true;
//      first = false;
//    }
//
//    action.driveTrain.mode = action.driveTrain.POSITION;
//    action.driveTrain.distanceSetPoint    = 6*12.0;
//    action.driveTrain.bearingSetPoint     = 0.0;
//
//
//
//    if(cycleCount >= 0 && ++cycleCount > 50 * 5)
//    {
//      action.driveTrain.usingClosedLoop = false;
//      action.driveTrain.mode = action.driveTrain.SPEED;
//
//      action.driveTrain.rawForward = 0.2;
//      action.driveTrain.rawTurn = 0.0;
//
//      if(++cycleCount > 50 * 6)
//      {
//          if(driveEncoders.GetNormalizedForwardSpeed()
//                  < 0.1)
//          {
//              action.driveTrain.usingClosedLoop = true;
//              action.driveTrain.mode = action.driveTrain.POSITION;
//
//              action.driveTrain.rawForward = 0.0;
//              action.driveTrain.rawTurn = 0.0;
//
//              action.driveTrain.resetFwd = true;
//              action.driveTrain.resetTurn = true;
//
//              action.driveTrain.distanceSetPoint  = -2*12.0;
//              action.driveTrain.bearingSetPoint   = 0.0;
//              cycleCount = -1;
//          }
//      }
//    }

//    action.driveTrain.rawForward = inputs.GetForward();
//    action.driveTrain.rawTurn    = inputs.GetTurn();
//
//    action.driveTrain.brakeLeft  = inputs.ShouldBrakeLeft();
//    action.driveTrain.brakeRight = inputs.ShouldBrakeRight();

//    action.driveTrain.thirdGear  = inputs.ShouldShiftThird();

}
