#include "Brain.h"

#define LIFT_RELEASE

void Brain::AutomatedRoutines()
{
    // setup for new release method using the arm
    if(inputs.ShouldCommenceMoveArmToMiddle())
        action.automatedRoutine.ringer = action.automatedRoutine.ARM_MIDDLE_POSITON;
#ifdef LIFT_RELEASE
    else if(inputs.ShouldCommenceReleaseRingerWithLift())
        action.automatedRoutine.ringer = action.automatedRoutine.COMMENCE_DROP_RINGER;
    else if(inputs.ShouldReleaseRingerWithLift())
        action.automatedRoutine.ringer = action.automatedRoutine.DROP_RINGER;
    else if(inputs.ShouldTerminateReleaseRingerWithLift())
        action.automatedRoutine.ringer = action.automatedRoutine.TERMINATE_DROP_RINGER;
#else
    else if(inputs.ShouldCommenceReleaseRingerWithArm())
        action.automatedRoutine.ringer = action.automatedRoutine.DROP_RINGER;
#endif
    else if(inputs.ShouldCommenceMoveArmUpAndLiftDown())
    {
        action.automatedRoutine.ringer = action.automatedRoutine.ARM_UP;

        // since the lift waits for the arm you have to make sure
        // it does not think that the arm is already done
        action.arm.doneState = action.arm.STALE;
    }

    // if aborted make sure we terminate the automated routine
    if(inputs.ShouldAbort())
        action.automatedRoutine.ringer = action.automatedRoutine.IDLE;

    // execution for new release method using the arm
    if(action.automatedRoutine.ringer == action.automatedRoutine.ARM_MIDDLE_POSITON)
        action.arm.state = action.arm.PRESET_MIDDLE;


#ifdef LIFT_RELEASE
    else if(action.automatedRoutine.ringer == action.automatedRoutine.COMMENCE_DROP_RINGER)
    {
        action.roller.commenceAutomation = true;
        action.roller.automated = true;
    }
#endif
    else if(action.automatedRoutine.ringer == action.automatedRoutine.DROP_RINGER)
    {
#ifdef LIFT_RELEASE
        action.roller.commenceAutomation = false;
        action.roller.automated = true;
#else
        action.arm.state = action.arm.PRESET_BOTTOM;
        action.roller.state = action.roller.SPITTING;
#endif
    }
#ifdef LIFT_RELEASE
    else if(action.automatedRoutine.ringer == action.automatedRoutine.TERMINATE_DROP_RINGER)
    {
        action.roller.automated = false;
        //automatically put the arm up and lift down
        action.automatedRoutine.ringer = action.automatedRoutine.ARM_UP;
    }
#endif


    else if(action.automatedRoutine.ringer == action.automatedRoutine.ARM_UP)
    {
        action.arm.state = action.arm.PRESET_TOP;
        if(action.arm.doneState == action.arm.SUCCESS)
        {
            AsynchronousPrinter::Printf("arm done\n");
            action.automatedRoutine.ringer = action.automatedRoutine.LIFT_DOWN;

            action.lift.givenCommand = true;
            action.lift.preset = action.lift.LOW_PEG;
            action.lift.doneState = action.lift.STALE;
        }
    }
    else if(action.automatedRoutine.ringer == action.automatedRoutine.LIFT_DOWN)
    {
        if(action.lift.doneState == action.lift.SUCCESS)
            action.automatedRoutine.ringer = action.automatedRoutine.IDLE;
    }
}

void Brain::AutomatedFollowLine()
{
    {
        ProfiledSection ps("Line Sensing");

        static enum
        {
            DETERMINING,
            FINDING,
            FOLLOWING,
            DONE
        } state = DETERMINING;

        // get the line position from the sensor
        int linePosition = lineSensor.GetLinePosition();
#ifdef USE_DASHBOARD
        SmartDashboard::Log(linePosition, "Line position");
#endif

        if(previousState == DISABLED)
        {
            // reset all flags and start over
            state = DETERMINING;
            prevLinePosition = LineSensor::LINE_NOT_DETECTED;
            firstReading = true;
        }

        // figure out which state to begin with
        if(state == DETERMINING)
        {
            if(linePosition == LineSensor::LINE_NOT_DETECTED)
                state = FINDING;
            else
                state = FOLLOWING;
        }

        switch(state)
        {
        case DETERMINING:
            // line sensing should not enter this state (see if block above)
            AsynchronousPrinter::Printf("Line sensing should not enter DETERMINING state\n");
            break;

        case FINDING:
            action.driveTrain.rate.rawForward = 0.2;
            action.driveTrain.rate.rawTurn = 0.0;

            if(firstReading)
            {
                // find out what side the robot is approaching from
                leftSide = linePosition > 66;
                firstReading = false;
            }
            // if the robot has moved over the center, start following
            else if((leftSide && linePosition < 66) || (!leftSide && linePosition > 66))
                state = FOLLOWING;
            break;

        case FOLLOWING:
            action.driveTrain.rate.rawForward = 0.4;

            // push line position to the extreme if the line isn't detected
            if(linePosition == LineSensor::LINE_NOT_DETECTED)
            {
                // go slower when trying to redetect line
                action.driveTrain.rate.rawForward = 0.2;

                // if the last value to the right of the center, use 80
                if(prevLinePosition > 50)
                    linePosition = 80;
                // otherwise use the low value, 20 (see rescaling below)
                else
                    linePosition = 20;
            }
            else if(linePosition == LineSensor::END_OF_LINE)
            {
                action.driveTrain.rate.rawForward = 0.0;
                action.driveTrain.rate.rawTurn = 0.0;
                state = DONE;
            }

            // first 4 pixels are sometimes bogus; they are cut out
            action.driveTrain.rate.rawTurn = -0.1 * Util::Rescale(linePosition, 20, 80, -1, 1);
            break;

        case DONE:
            // nothing to do
            break;
        }

        prevLinePosition = linePosition;
#ifdef USE_DASHBOARD
        SmartDashboard::Log(linePosition, "Calculated Line position");
#endif
    }
}
