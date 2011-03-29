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
    	SET_ZERO_POINT,
    	DRIVE_FORWARD,
    	STALL_DETECTION,
    	SET_SECOND_ZERO_POINT,
    	DRIVE_BACK,
    	IDLE
    } state = SET_ZERO_POINT;
    
    static int timer = 0;
    
    AsynchronousPrinter::Printf("state:%d\n", (int) state);
    switch(state)
    {
    case SET_ZERO_POINT:
    	action.driveTrain.resetFwd = true;
    	action.driveTrain.resetTurn = true;
    	
    	action.driveTrain.mode = action.driveTrain.POSITION;
    	action.driveTrain.distanceSetPoint = 6.0 * 12;
    	action.driveTrain.bearingSetPoint  = 0.0;
    	
    	timer = 0;
    	state = DRIVE_FORWARD;
    	break;
    	
    case DRIVE_FORWARD:
    	action.driveTrain.resetFwd = false;
    	action.driveTrain.resetTurn = false;
    	
    	if(++timer > 50 * 5)
    	{
    		wait = 50;
    		timer = 0;
    		state = STALL_DETECTION;
    	}
    	break;
    	
    case STALL_DETECTION:
    	action.driveTrain.mode = action.driveTrain.SPEED;
    	action.driveTrain.usingClosedLoop = false;
    	
    	action.driveTrain.rawForward = 0.2;
    	action.driveTrain.rawTurn = 0.0;
    	
    	if(++timer > 50)
    	{
    		if(driveEncoders.GetNormalizedForwardSpeed()
    				< 0.1)
    		{
    			wait = 50;
    			action.driveTrain.rawForward = 0.0;
    			action.driveTrain.rawTurn = 0.0;
    			state = SET_SECOND_ZERO_POINT;
    		}
    	}
    	break;
    	
    case SET_SECOND_ZERO_POINT:
    	action.driveTrain.usingClosedLoop = true;
    	action.driveTrain.mode = action.driveTrain.POSITION;

    	action.driveTrain.rawForward = 0.0;
    	action.driveTrain.rawTurn = 0.0;

    	action.driveTrain.resetFwd = true;
    	action.driveTrain.resetTurn = true;
    	
    	action.driveTrain.distanceSetPoint  = -6.0;
    	action.driveTrain.bearingSetPoint 	= 0.0;
    	
    	timer = 0;
    	state = DRIVE_BACK;
    	break;
    	
    case DRIVE_BACK:
    	action.driveTrain.resetFwd = false;
    	action.driveTrain.resetTurn = false;
    	
    	action.driveTrain.maxFwdSpeed = 0.15;

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
//    	wait--;
//    }
//    
//    static int cycleCount = 0;
//    static bool first = true;
//    if (first || cycleCount == -1)
//    {
//    	action.driveTrain.resetFwd  = true;
//    	action.driveTrain.resetTurn = true;
//    	first = false;
//    }
//    
//    action.driveTrain.mode = action.driveTrain.POSITION;
//    action.driveTrain.distanceSetPoint 	= 6*12.0;
//    action.driveTrain.bearingSetPoint 	= 0.0;
//
//    
//    
//    if(cycleCount >= 0 && ++cycleCount > 50 * 5)
//    {
//    	action.driveTrain.usingClosedLoop = false;
//    	action.driveTrain.mode = action.driveTrain.SPEED;
//    	
//    	action.driveTrain.rawForward = 0.2;
//    	action.driveTrain.rawTurn = 0.0;
//    	
//    	if(++cycleCount > 50 * 6)
//    	{
//    		if(driveEncoders.GetNormalizedForwardSpeed()
//    				< 0.1)
//    		{
//    			action.driveTrain.usingClosedLoop = true;
//    			action.driveTrain.mode = action.driveTrain.POSITION;
//    			
//    			action.driveTrain.rawForward = 0.0;
//    			action.driveTrain.rawTurn = 0.0;
//    			
//    			action.driveTrain.resetFwd = true;
//    			action.driveTrain.resetTurn = true;
//    			
//    			action.driveTrain.distanceSetPoint  = -2*12.0;
//    			action.driveTrain.bearingSetPoint 	= 0.0;
//    			cycleCount = -1;
//    		}
//    	}
//    }

//    action.driveTrain.rawForward = inputs.GetForward();
//    action.driveTrain.rawTurn    = inputs.GetTurn();
//
//    action.driveTrain.brakeLeft  = inputs.ShouldBrakeLeft();
//    action.driveTrain.brakeRight = inputs.ShouldBrakeRight();

//    action.driveTrain.thirdGear  = inputs.ShouldShiftThird();

}
