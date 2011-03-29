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

        if(action.driveTrain.distance.done)
        {
            action.driveTrain.mode = action.driveTrain.RATE;
            action.driveTrain.rate.rawForward = 0.0;
            action.driveTrain.rate.rawTurn = 0.0;
        }
    }

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

    static enum
    {
        SET_COMMAND,
        DRIVE_FORWARD,
        STALL_DETECTION,
        SET_SECOND_COMMAND,
        STEP_BACK,
        SET_THIRD_COMMAND,
        DRIVE_BACK,
        TURN_AROUND,
        IDLE
    } state = SET_COMMAND;

    static int timer = 0;

    if(wasDisabled)
    {
        timer = 0;
        state = SET_COMMAND;
    }

    switch(state)
    {
    case SET_COMMAND:
        action.driveTrain.mode = action.driveTrain.DISTANCE;
        action.driveTrain.distance.givenCommand = true;

        action.driveTrain.distance.distanceSetPoint = 6.0 * 12; // 6 feet
        action.driveTrain.distance.distanceDutyCycle = 0.5;

        action.driveTrain.distance.done = false;
        state = DRIVE_FORWARD;
        break;

    case DRIVE_FORWARD:
        if(action.driveTrain.distance.done)
        {
            timer = 0;
            state = STALL_DETECTION;
        }
        break;

    case STALL_DETECTION:
        action.driveTrain.mode = action.driveTrain.RATE;
        action.driveTrain.rate.usingClosedLoop = false;

        action.driveTrain.rate.rawForward = 0.2;
        action.driveTrain.rate.rawTurn = 0.0;

        if(++timer > 25)
        {
            if(driveEncoders.GetNormalizedForwardSpeed()
                    < 0.05)
                state = SET_SECOND_COMMAND;
        }
        break;

    case SET_SECOND_COMMAND:
        action.driveTrain.rate.usingClosedLoop = true;
        action.driveTrain.rate.rawForward = 0.0;
        action.driveTrain.rate.rawTurn = 0.0;

        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = true;
        action.driveTrain.position.shouldTurnAngle = false;

        action.driveTrain.position.distanceSetPoint = -6.0; // 6 inches back
        action.driveTrain.position.turnSetPoint = 0.0;

        action.driveTrain.position.maxFwdSpeed = 0.15;
        action.driveTrain.position.maxTurnSpeed = 1.0;

        timer = 0;
        state = STEP_BACK;
        break;

    case STEP_BACK:
        // wait one second for driving to finish
        if(++timer > 50)
            state = SET_THIRD_COMMAND;
        break;

    case SET_THIRD_COMMAND:
        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = true;
        action.driveTrain.position.shouldTurnAngle = false;

        action.driveTrain.position.distanceSetPoint = -6.0 * 12; // 6 feet back
        action.driveTrain.position.turnSetPoint = 0.0;

        action.driveTrain.position.maxFwdSpeed = 0.3;
        action.driveTrain.position.maxTurnSpeed = 1.0;

        timer = 0;
        state = DRIVE_BACK;
        break;

    case DRIVE_BACK:
        if(++timer > 150)
            state = TURN_AROUND;
        break;

    case TURN_AROUND:
        action.driveTrain.mode = action.driveTrain.POSITION;
        action.driveTrain.position.givenCommand = true;

        action.driveTrain.position.shouldMoveDistance = false;
        action.driveTrain.position.shouldTurnAngle = true;

        action.driveTrain.position.distanceSetPoint = 0.0;
        action.driveTrain.position.turnSetPoint = 180.0; // 180 degrees

        action.driveTrain.position.maxFwdSpeed = 1.0;
        action.driveTrain.position.maxTurnSpeed = 1.0;

        state = IDLE;
        break;

    case IDLE:
        // wait for turning to complete and do nothing
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

//    action.driveTrain.mode = action.driveTrain.RATE;
//    action.driveTrain.rawForward = inputs.GetForward();
//    action.driveTrain.rawTurn    = inputs.GetTurn();
//
//    action.driveTrain.brakeLeft  = inputs.ShouldBrakeLeft();
//    action.driveTrain.brakeRight = inputs.ShouldBrakeRight();

//    action.driveTrain.thirdGear  = inputs.ShouldShiftThird();

}
