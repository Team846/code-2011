#include "Brain.h"

void Brain::Auton()
{
//    // TODO remove return statement after auton is legitimate
//    return;
//
//    // robot starts at either a side or the middle
//    static enum
//    {
//        SIDE,
//        MIDDLE,
//        DUMB
//    } startPosition;
//
//    // position determined by digital inputs
//    if(ds.GetDigitalIn(1))
//        startPosition = SIDE;
//    else if(ds.GetDigitalIn(2))
//        startPosition = MIDDLE;
//    else
//        startPosition = DUMB;
//
//    // call state-specific methods
//    switch(startPosition)
//    {
//    case SIDE:
//        Side();
//        break;
//    case MIDDLE:
//        Middle(ds.GetDigitalIn(2) ? 2 : 1);
//        break;
//    case DUMB:
    EncoderAuton();
//        break;
//    }
//
//    // includes automated routines such as line sensing and
//    // dead-reckoning autonomous mode
////    AutomatedRoutines();
}

void Brain::EncoderAuton()
{
    static enum
    {
        CONFIGURE,
        DRIVE,
        WAIT_FOR_DRIVE,
        BACK_UP,
        WAIT_FOR_BACK_UP,
//        MOVE_LIFT_UP,
//        WAIT_FOR_MOVE_LIFT_UP,
//        RELEASE_ROLLER,
//        WAIT_FOR_RELEASE_ROLLER,
        DONE
    } state = CONFIGURE;

    static int counter = 0;

    switch(state)
    {
    case CONFIGURE:
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        state = DRIVE;
        break;

    case DRIVE:
        action.driveTrain.usingClosedLoop = false;
        action.driveTrain.rawForward = 0.5;
        action.driveTrain.rawTurn = 0.0;

        state = WAIT_FOR_DRIVE;
        counter = 0;
        break;

    case WAIT_FOR_DRIVE:
        // after 1s
        if(++counter > 25)
        {
            AsynchronousPrinter::Printf("Low gear speed: %.2f\n",
                    driveEncoders.GetNormalizedLowGearForwardSpeed());

            if(driveEncoders.GetNormalizedLowGearForwardSpeed()
                    < action.driveTrain.rawForward - 0.2)
            {
                action.driveTrain.rawForward = 0.0;
                action.driveTrain.rawTurn = 0.0;
                action.driveTrain.usingClosedLoop = true;
                state = BACK_UP;
            }
        }
        break;

    case BACK_UP:
        action.driveTrain.rawForward = -0.2;
        action.driveTrain.rawTurn = 0.0;

        state = WAIT_FOR_BACK_UP;
        counter = 0;
        break;

    case WAIT_FOR_BACK_UP:
        float target = driveEncoders.GetRobotDist() - 2;
        AsynchronousPrinter::Printf("Target distance: %.2f\n", target);
        if(driveEncoders.GetRobotDist() > target)
        {
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.rawTurn = 0.0;
            state = DONE;
//          state = MOVE_LIFT_UP;
        }
        break;


//  case MOVE_LIFT_UP:
//      action.lift.givenCommand = true;
//        if (ds.GetDigitalIn(8))
//          action.lift.highRow = true; // going from the side
//        else
//          action.lift.highRow = false; // going from the side
//
//        action.lift.preset = action.lift.HIGH_PEG;
//        state = WAIT_FOR_MOVE_LIFT_UP;
//      break;
//
//  case WAIT_FOR_MOVE_LIFT_UP:
//      if(action.lift.doneState != action.lift.STALE) // message is available
//        {
//            if(action.lift.doneState == action.lift.SUCCESS)
//                state = RELEASE_ROLLER;
//            else
//              state = DONE;
//        }
//      break;
//
//  case RELEASE_ROLLER:
//      action.roller.automated = true;
//      action.roller.commenceAutomation = true;
//      state = WAIT_FOR_RELEASE_ROLLER;
//      counter = 0;
//      break;
//
//  case WAIT_FOR_RELEASE_ROLLER:
//      action.roller.commenceAutomation = false;
//
//      // one second of reversing the roller
//      if(++counter >= 50)
//      {
//          // stop automating; stop rollers
//          action.roller.automated = false;
//          state = DONE;
//
//          counter = 0; // reset timer
//      }
//      break;
//
    case DONE:
        // do nothing
        break;

    }
}
