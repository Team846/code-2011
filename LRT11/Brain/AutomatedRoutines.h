#include "Brain.h"

void Brain::AutomatedRoutines()
{
    // used for state machine
    /* old routine
    static enum
    {
        IDLE,
        ROTATING_RINGER,
        START_SPITTING,
        MOVE_LIFT_UP,
        MOVE_LIFT_DOWN,
        WAIT_FOR_LIFT,
        END_SPITTING,
        ABORT
    } state = IDLE;

    switch(state)
    {
    case IDLE:
        if(action.automation.releaseRinger)
            state = ROTATING_RINGER;
        break;

    case ROTATING_RINGER:
        static int rotateCount = 0;
        action.roller.rotateUpward = false;
        action.roller.state = action.roller.ROTATING;

        // one second of rotating
        if(++rotateCount % 50 == 0)
        {
            state = START_SPITTING;
            action.roller.state = action.roller.STOPPED;
        }
        break;

    case START_SPITTING:
        action.roller.state = action.roller.SPITTING;
        state = MOVE_LIFT_UP;
        break;

    case MOVE_LIFT_UP:
        // configuration for moving to a preset
        action.lift.givenCommand = true;
        action.lift.preset = action.lift.HIGH_PEG;
        action.lift.highRow = false; // should change based off position

        state = WAIT_FOR_LIFT;
        break;

    case MOVE_LIFT_DOWN:
        // configuration for moving to a preset
        action.lift.givenCommand = true;
        action.lift.preset = action.lift.STOWED;
        action.lift.highRow = false; // should change based off position

        state = WAIT_FOR_LIFT;
        break;

    case WAIT_FOR_LIFT:
        // wait until lift is done
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = END_SPITTING;
            else if(action.lift.doneState == action.lift.FAILURE)
                state = ABORT; // TODO correct state
            else if(action.lift.doneState == action.lift.ABORTED)
                state = ABORT; // TODO correct state
        }
        break;

    case END_SPITTING:
        action.roller.state = action.roller.STOPPED;
        action.automation.releaseRinger = false;
        state = IDLE; // wait for next command
        break;

    case ABORT:
        // TODO abort
        break;
    }
    */
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

                // if the last value to the right of the center, use 128
                if(prevLinePosition > 66)
                    linePosition = 128;
                // otherwise use the low value, 4 (see rescaling below)
                else
                    linePosition = 4;
            }
            else if(linePosition == LineSensor::END_OF_LINE)
            {
                action.driveTrain.rate.rawForward = 0.0;
                action.driveTrain.rate.rawTurn = 0.0;
                state = DONE;
            }

            // first 4 pixels are sometimes bogus; they are cut out
            action.driveTrain.rate.rawTurn = -0.1 * Util::Rescale(linePosition, 4, 128, -1, 1);
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
