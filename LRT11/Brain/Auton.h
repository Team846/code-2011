#include "Brain.h"

void Brain::Auton()
{
    // TODO remove return statement after auton is legitimate
    return;

    // robot starts at either a side or the middle
    static enum
    {
        SIDE,
        MIDDLE,
        DUMB
    } startPosition;

    // position determined by digital inputs
    if(ds.GetDigitalIn(1))
        startPosition = SIDE;
    else if (ds.GetDigitalIn(2))
        startPosition = MIDDLE;
    else 
    	startPosition = DUMB;

    // call state-specific methods
    switch(startPosition)
    {
    case SIDE:
        Side();
        break;
    case MIDDLE:
        Middle(ds.GetDigitalIn(2) ? 2 : 1);
        break;
    case DUMB:
    	EncoderAuton();
    	break;
    }

    // includes automated routines such as line sensing and
    // dead-reckoning autonomous mode
//    AutomatedRoutines();
}

void Brain::Middle(int numberOfTubes)
{
    static enum
    {
        dCONFIGURE,

        DRIVE_FORWARD,
        WAIT_FOR_DRIVE_TRAIN,

        dFOLLOW_LINE,
        dMOVE_LIFT_UP,
        RELEASE_RINGER,
        dMOVE_LIFT_DOWN,
        TURN_AROUND
    } dstate = dCONFIGURE;

    switch(dstate)
    {
    case dCONFIGURE:
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        action.positionTrain.enabled = true;
        dstate = DRIVE_FORWARD;
        break;

    case DRIVE_FORWARD:
        action.positionTrain.shouldMoveDistance = true;
        // placeholder; distance to pegs in inches
        action.positionTrain.moveDistance = 16.75 * 12;

        dstate = WAIT_FOR_DRIVE_TRAIN;
        break;

    case WAIT_FOR_DRIVE_TRAIN:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            dstate = dFOLLOW_LINE;
        }
        break;

    case dFOLLOW_LINE:
        AutomatedFollowLine();
        break;

    case dMOVE_LIFT_UP:
        break;

    case RELEASE_RINGER:
        break;

    case dMOVE_LIFT_DOWN:
        break;

    case TURN_AROUND:
        break;

    }

    return;

    static bool isOnLeftSide = false;
    static bool isFirstRinger = true;
    static double startDist = 0.0;
    static int timer = 0;

    static enum
    {
        CONFIGURE,

        DRIVE_TO_Y,
        FOLLOW_Y,
        MOVE_LIFT_UP,
        RELEASE_TUBE,
        MOVE_LIFT_DOWN,
        TURN_TO_GO_BACK,
        MOVE_BACK,
        TURN_TO_PICKUP_RINGER,
        DRIVE_TO_LINE, // grab ringer
        FOLLOW_LINE,
        BACK_OFF,

        WAIT_FOR_DRIVE_TO_Y,
        WAIT_FOR_FOLLOW_Y,
        WAIT_FOR_MOVE_LIFT_UP,
        WAIT_FOR_RELEASE_TUBE,
        WAIT_FOR_MOVE_LIFT_DOWN,
        WAIT_FOR_TURN_TO_GO_BACK,
        WAIT_FOR_MOVE_BACK,
        WAIT_FOR_TURN_TO_PICKUP_RINGER,
        WAIT_FOR_DRIVE_TO_LINE,
        WAIT_FOR_FOLLOW_LINE,
        WAIT_FOR_BACK_OFF,

        IDLE
    } state = CONFIGURE;

    switch(state)
    {
    case CONFIGURE:
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;
        isFirstRinger = true;

        state = DRIVE_TO_Y;
        break;

        //Stop a little short (0.65 ft) of the leg of the Y
    case DRIVE_TO_Y:
        // switch to low gear and shift
        action.driveTrain.rawForward = 0.8;
        action.driveTrain.rawTurn = 0.0;

        state = WAIT_FOR_DRIVE_TO_Y;
        break;

    case FOLLOW_Y:
        startDist = driveEncoders.GetRobotDist();
        state = WAIT_FOR_FOLLOW_Y;
        break;

    case MOVE_LIFT_UP:
        action.lift.givenCommand = true;
        action.lift.highRow = isFirstRinger;

        action.lift.preset = action.lift.HIGH_PEG;

        state = WAIT_FOR_MOVE_LIFT_UP;
        break;

    case RELEASE_TUBE:
        action.roller.automated = true;
        action.roller.commenceAutomation = true;
        state = WAIT_FOR_RELEASE_TUBE;
        break;

    case MOVE_LIFT_DOWN:
        action.lift.givenCommand = true;
        action.lift.highRow = false;
        action.lift.preset = action.lift.LOW_PEG;

        state = WAIT_FOR_MOVE_LIFT_DOWN;
        break;

    case TURN_TO_GO_BACK:
        action.positionTrain.enabled = true;
        action.positionTrain.shouldTurnAngle = true;

        if(driveEncoders.GetTurnAngle() > 0)  //TODO CHECKME! These may be wrong.
        {
            isOnLeftSide = true;
            action.positionTrain.pivotLeft = true;
            action.positionTrain.turnAngle = -45;
        }
        else
        {
            isOnLeftSide = false;
            action.positionTrain.pivotRight = true;
            action.positionTrain.turnAngle  = 45;
        }

        state = WAIT_FOR_TURN_TO_GO_BACK;
        break;

    case MOVE_BACK:
        action.positionTrain.enabled = true;
        action.positionTrain.shouldMoveDistance = true;

        // placeholder; distance to pegs in inches
        action.positionTrain.moveDistance = -20.5 * 12;

        state = WAIT_FOR_MOVE_BACK;
        break;

    case TURN_TO_PICKUP_RINGER:
        action.positionTrain.enabled = true;
        action.positionTrain.shouldTurnAngle = true;

        if(isOnLeftSide)  //TODO CHECKME! These may be wrong.
            action.positionTrain.turnAngle = 45;
        else
            action.positionTrain.turnAngle  = -45;

        state = WAIT_FOR_TURN_TO_PICKUP_RINGER;
        break;

    case DRIVE_TO_LINE:
        action.driveTrain.rawForward = 0.8;
        action.driveTrain.rawTurn = 0.0;

        //move arm down + suck in
        action.roller.state = action.roller.SUCKING;
        action.arm.presetTop = false;
        action.arm.givenCommand = true;
        action.arm.manualMode = false;
        action.arm.doneState = action.arm.STALE;

        startDist = driveEncoders.GetRobotDist();
        state = WAIT_FOR_DRIVE_TO_LINE;
        break;

    case FOLLOW_LINE:
        action.roller.state = action.roller.STOPPED;
        action.arm.presetTop = true;
        action.arm.givenCommand = true;
        action.arm.manualMode = false;
        action.arm.doneState = action.arm.STALE;

        startDist = driveEncoders.GetRobotDist();
        state = WAIT_FOR_FOLLOW_LINE;
        break;

    case BACK_OFF:
        action.positionTrain.enabled = true;
        action.positionTrain.moveDistance = -6;
        state = WAIT_FOR_BACK_OFF;
        break;

    case WAIT_FOR_DRIVE_TO_Y:
        if(driveEncoders.GetRobotDist() > 16.75 * 12) //based on field drawings
        {
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.rawTurn = 0.0;
            state = FOLLOW_Y;
        }
        break;

    case WAIT_FOR_FOLLOW_Y:
        AutomatedFollowLine();//Follow the line

        //when close to to the pole disable closed loop and enable stall detection
        if(driveEncoders.GetRobotDist() > 20) //TODO Find the val empirically, maybe reduce speed
            action.driveTrain.usingClosedLoop = false;

        if(driveEncoders.GetNormalizedForwardSpeed() < 0.2) //TODO tweak This means we have crashed into the pole
        {
            action.driveTrain.usingClosedLoop = true;
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.rawTurn = 0.0;
            state = MOVE_LIFT_UP;
        }

        break;

    case WAIT_FOR_MOVE_LIFT_UP:
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = RELEASE_TUBE;
            else if(action.lift.doneState == action.lift.FAILURE)
                state = IDLE; // TODO correct state
            else if(action.lift.doneState == action.lift.ABORTED)
                state = IDLE; // TODO correct state
        }
        break;

    case WAIT_FOR_RELEASE_TUBE:
        action.roller.commenceAutomation = false;

        // one second of reversing the roller
        if(++timer % 50 == 0)
        {
            // stop automating; stop rollers
            action.roller.automated = false;
            state = MOVE_LIFT_DOWN;

            timer = 0; // reset timer
        }
        break;

    case WAIT_FOR_MOVE_LIFT_DOWN:
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                if(isFirstRinger)
                {
                    state = TURN_TO_GO_BACK;
                    isFirstRinger = false;
                }
                else
                    state = BACK_OFF;
            else if(action.lift.doneState == action.lift.FAILURE)
                state = IDLE; // TODO correct state
            else if(action.lift.doneState == action.lift.ABORTED)
                state = IDLE; // TODO correct state
        }
        break;

    case WAIT_FOR_TURN_TO_GO_BACK:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            // should move distance is automatically reset
            action.positionTrain.pivotLeft = false;
            action.positionTrain.pivotRight = false;

            state = MOVE_BACK;
        }
        break;

    case WAIT_FOR_MOVE_BACK:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            // should move distance is automatically reset

            if(numberOfTubes == 1)
                state = IDLE;
            else
                state = TURN_TO_PICKUP_RINGER;
        }
        break;

    case WAIT_FOR_TURN_TO_PICKUP_RINGER:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            // should move distance is automatically reset

            state = DRIVE_TO_LINE;
        }
        break;

    case WAIT_FOR_DRIVE_TO_LINE:
        if(driveEncoders.GetRobotDist() - startDist > 75)
        {
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.rawTurn = 0.0;

            state = FOLLOW_LINE;
        }
        break;


    case WAIT_FOR_FOLLOW_LINE:
        AutomatedFollowLine();//Follow the line
        if(driveEncoders.GetRobotDist() > 12) //TODO FIGURE ME OUT!
            action.driveTrain.usingClosedLoop = false;
        if(driveEncoders.GetNormalizedForwardSpeed() < 0.2)
        {
            state = MOVE_LIFT_UP;
            action.driveTrain.usingClosedLoop = true;
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.rawTurn = 0.0;
        }
        break;
    case WAIT_FOR_BACK_OFF:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            state = IDLE;
        }
        break;

    case IDLE:
        //ne faire rien
        break;
    }
}

void Brain::Side()
{
    // used for state machine
    static enum
    {
        DRIVE_FORWARD,
        WAIT_FOR_DRIVE,
        RUN_INTO_PEG,
        WAIT_FOR_PEG,
        MOVE_LIFT_UP,
        WAIT_FOR_LIFT,
        RELEASE_TUBE,
        WAIT_FOR_RELEASE,
        MOVE_LIFT_DOWN,
        TURN_AROUND,
        DRIVE_LAST_STRETCH,
        DONE
    } state = DRIVE_FORWARD
    , nextState = DONE;

    static int timer = 0;
    switch(state)
    {
    case DRIVE_FORWARD:
        // switch to low gear and shift
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        action.positionTrain.enabled = true;
        action.positionTrain.shouldMoveDistance = true;

        // placeholder; distance to pegs in inches
        action.positionTrain.moveDistance = 36.0;

        state = WAIT_FOR_DRIVE;
        nextState = RUN_INTO_PEG;
        break;

    case WAIT_FOR_DRIVE:
        if(action.positionTrain.done)
        {
            action.positionTrain.enabled = false;
            // should move distance is automatically reset

            // next state determined by previous state (see DRIVE_FORWARD
            // for an example)
            state = nextState;
        }
        break;

    case RUN_INTO_PEG:
        action.driveTrain.usingClosedLoop = false;

#define FORWARD_RATE 0.1
        action.driveTrain.rawForward = FORWARD_RATE;
        action.driveTrain.rawTurn = 0.0;

        state = WAIT_FOR_PEG;
        break;

    case WAIT_FOR_PEG:
        // TODO: FIX
        // if encoder rate is less than one half of specified rate, assume
        // that the drive train has stalled
        if(DriveEncoders::GetInstance().GetNormalizedForwardSpeed() < FORWARD_RATE / 2.0)
        {
            action.driveTrain.rawForward = 0.0;
            action.driveTrain.usingClosedLoop = true;
            state = MOVE_LIFT_UP;
        }
#undef FORWARD_RATE
        break;

    case MOVE_LIFT_UP:
        action.lift.givenCommand = true;
        action.lift.highRow = false; // going from the side
        action.lift.preset = action.lift.HIGH_PEG;

        state = WAIT_FOR_LIFT;
        nextState = RELEASE_TUBE;
        break;

    case WAIT_FOR_LIFT:
        // wait until lift is done
        if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = nextState;
            else if(action.lift.doneState == action.lift.FAILURE ||
                    action.lift.doneState == action.lift.ABORTED)
                // error; put down lift and end autonomous mode
                state = MOVE_LIFT_DOWN;
        }
        break;

    case RELEASE_TUBE:
        // start the roller automation to release the tube
        action.roller.automated = true;
        action.roller.commenceAutomation = true;
        state = WAIT_FOR_RELEASE;
        break;

    case WAIT_FOR_RELEASE:
        // already commenced automation
        action.roller.commenceAutomation = false;

        // one second of reversing the roller
        if(++timer % 50 == 0)
        {
            // stop automating; stop rollers
            action.roller.automated = false;
            state = MOVE_LIFT_DOWN;

            timer = 0; // reset timer
        }
        break;

    case MOVE_LIFT_DOWN:
        action.lift.givenCommand = true;
        action.lift.highRow = false; // going from the side
        action.lift.preset = action.lift.LOW_PEG;

        // could cause cyclic state change from MOVE_LIFT_DOWN <->
        // WAIT_FOR_LIFT, but this is unlikely to occur and will not
        // cause a significant failure (lift timeout will prevent
        // damage and autonomous mode will end)
        state = WAIT_FOR_LIFT;
        nextState = TURN_AROUND;
        break;

    case TURN_AROUND:
        action.positionTrain.enabled = true;
        action.positionTrain.shouldTurnAngle = true;

        action.positionTrain.turnAngle = 180.0; // degrees

        state = WAIT_FOR_DRIVE;
        nextState = DRIVE_LAST_STRETCH;
        break;

    case DRIVE_LAST_STRETCH:
        action.positionTrain.enabled = true;
        action.positionTrain.shouldMoveDistance = true;

        // placeholder; distance to pegs in inches
        action.positionTrain.moveDistance = 36.0;

        state = WAIT_FOR_DRIVE;
        nextState = DONE;
        break;

    case DONE:
        // done!
        break;
    }


}

void Brain::EncoderAuton()
{
	static enum 
	{
		CONFIGURE,
		DRIVE,
		WAIT_FOR_DRIVE,
		MOVE_LIFT_UP,
		WAIT_FOR_MOVE_LIFT_UP,
		RELEASE_ROLLER,
		WAIT_FOR_RELEASE_ROLLER,
		DONE
	}state ;
	
	static int counter = 0;
	
	switch (state)
	{
	case CONFIGURE:
        action.shifter.gear = action.shifter.LOW_GEAR;
        action.shifter.force = true;

        action.positionTrain.enabled = true;
        state = DRIVE;
        counter = 0;
		break;
	
	case DRIVE:
		action.driveTrain.usingClosedLoop = false;
		action.driveTrain.rawForward = 0.5;
		action.driveTrain.rawTurn = 0.0;
		break;
		
	case WAIT_FOR_DRIVE:
		// after 1s
		if (++counter > 50 && 
				driveEncoders.GetNormalizedLowGearForwardSpeed() 
					< action.driveTrain.rawForward - 0.3)
		{
			action.driveTrain.rawForward = 0.0;
			action.driveTrain.rawTurn = 0.0;
			action.driveTrain.usingClosedLoop = true;
			state = MOVE_LIFT_UP;
		}
		break;
		
	case MOVE_LIFT_UP:
		action.lift.givenCommand = true;
        if (ds.GetDigitalIn(8))
        	action.lift.highRow = true; // going from the side
        else
        	action.lift.highRow = false; // going from the side
        
        action.lift.preset = action.lift.HIGH_PEG;
        state = WAIT_FOR_MOVE_LIFT_UP;
		break;
		
	case WAIT_FOR_MOVE_LIFT_UP:
		if(action.lift.doneState != action.lift.STALE) // message is available
        {
            if(action.lift.doneState == action.lift.SUCCESS)
                state = RELEASE_ROLLER;
            else
            	state = DONE;
        }
		break;
		
	case RELEASE_ROLLER:
		action.roller.automated = true;
	    action.roller.commenceAutomation = true;
	    state = WAIT_FOR_RELEASE_ROLLER;
	    counter = 0;
		break;
		
	case WAIT_FOR_RELEASE_ROLLER:
		action.roller.commenceAutomation = false;
	
	    // one second of reversing the roller
	    if(++counter >= 50)
	    {
	        // stop automating; stop rollers
	        action.roller.automated = false;
	        state = DONE;
	
	        counter = 0; // reset timer
	    }	
		break;

	case DONE:
		// do nothing
		break;
		
	}
}
