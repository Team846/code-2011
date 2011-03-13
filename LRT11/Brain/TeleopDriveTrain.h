#include "Brain.h"

void Brain::TeleopDriveTrain()
{
//    action.driveTrain.rawForward = inputs.GetForward();
//    action.driveTrain.rawTurn    = inputs.GetTurn();

    {
        ProfiledSection ps("Line sensing");

        static enum
        {
            DETERMINING,
            FINDING,
            FOLLOWING,
            DONE
        } state = DETERMINING;

        int linePosition = lineSensor.GetLinePosition();
        SmartDashboard::Log(linePosition, "Line position");

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
            action.driveTrain.rawForward = -0.2;
            action.driveTrain.rawTurn = 0.0;

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
            action.driveTrain.rawForward = -0.4;

            // push line position to the extreme if the line isn't detected
            if(linePosition == LineSensor::LINE_NOT_DETECTED)
            {
                // go slower when trying to redetect line
                action.driveTrain.rawForward = -0.1;

                // if the last value to the right of the center, use 128
                if(prevLinePosition > 66)
                    linePosition = 128;
                // otherwise use the low value, 4 (see rescaling below)
                else
                    linePosition = 4;
            }
            else if(linePosition == LineSensor::END_OF_LINE)
            {
                action.driveTrain.rawForward = 0.0;
                action.driveTrain.rawTurn = 0.0;
                state = DONE;
            }

            // first 4 pixels are sometimes bogus; they are cut out
            action.driveTrain.rawTurn = 1 * Util::Rescale(linePosition, 4, 128, -1, 1);
            break;

        case DONE:
            // nothing to do
            break;
        }

        /*
        // drive forward (backward for 2010 robot) slowly
        action.driveTrain.rawForward = -0.4;

        // push line position to the extreme if the line isn't detected
        if(linePosition == -1)
        {
            // prevLinePosition has not been set and the line hasn't yet been
            // seen; drive forward until the line is detected
            if(prevLinePosition == -1)
            {
                // keep line position as -1 (see comment after this block)
                // redundant for clarity
                linePosition = -1;
                action.driveTrain.rawForward = -0.2;
            }
            // if the last value to the right of the center, use 128
            else if(prevLinePosition > 66)
            {
                action.driveTrain.rawForward = -0.1;
        //                action.driveTrain.brakeLeft = true;
                linePosition = 128;
            }
            // otherwise use the low value, 4 (see rescaling below)
            else
            {
                action.driveTrain.rawForward = -0.1;
        //                action.driveTrain.brakeRight = true;
                linePosition = 4;
            }
        }

        // if the robot is detecting the line for the first time
        if(prevLinePosition == -1 && linePosition != -1)
        {
            if(firstReading)
            {
                // find out what side the robot is approaching from
                leftSide = linePosition > 66;
                firstReading = false;
            }
            else
            {
                // if the robot has moved over the center, set prevLinePosition to start
                // tracking
                if((leftSide && linePosition < 66) || (!leftSide && linePosition > 66))
                    prevLinePosition = linePosition;
                // make sure prevLinePosition continues to be -1 (no line detected)
                else
                {
                    linePosition = -1;
                    action.driveTrain.rawForward = -0.2;
                }
            }
        }

        // linePosition will be set to a non-negative value at this point
        // unless we have never detected a line, at which point we should
        // keep driving forward
        if(linePosition == -1)
            action.driveTrain.rawTurn = 0;
        else
        {
            // first 4 pixels are sometimes bogus; they are cut out
            action.driveTrain.rawTurn = 1 * Util::Rescale(linePosition, 4, 128, -1, 1);
        }
        */

        prevLinePosition = linePosition;
        SmartDashboard::Log(linePosition, "Calculated Line position");
    }

    action.driveTrain.brakeLeft  = inputs.ShouldBrakeLeft();
    action.driveTrain.brakeRight = inputs.ShouldBrakeRight();
}
