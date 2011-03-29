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

//    action.driveTrain.speed.rawForward = 0.1;
//    action.driveTrain.speed.rawTurn = 0.0;
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
    	action.driveTrain.position.resetFwd = true;
    	action.driveTrain.position.resetTurn = true;
    	
    	action.driveTrain.mode = action.driveTrain.POSITION;
    	action.driveTrain.position.distanceSetPoint = 6.0 * 12;
    	action.driveTrain.position.bearingSetPoint  = 0.0;
    	
    	timer = 0;
    	state = DRIVE_FORWARD;
    	break;
    	
    case DRIVE_FORWARD:
    	action.driveTrain.position.resetFwd = false;
    	action.driveTrain.position.resetTurn = false;
    	
    	if(++timer > 50 * 5)
    	{
    		wait = 50;
    		timer = 0;
    		state = STALL_DETECTION;
    	}
    	break;
    	
    case STALL_DETECTION:
    	action.driveTrain.mode = action.driveTrain.SPEED;
    	action.driveTrain.speed.usingClosedLoop = false;
    	
    	action.driveTrain.speed.rawForward = 0.2;
    	action.driveTrain.speed.rawTurn = 0.0;
    	
    	if(++timer > 50)
    	{
    		if(driveEncoders.GetNormalizedForwardSpeed()
    				< 0.1)
    		{
    			wait = 50;
    			action.driveTrain.speed.rawForward = 0.0;
    			action.driveTrain.speed.rawTurn = 0.0;
    			state = SET_SECOND_ZERO_POINT;
    		}
    	}
    	break;
    	
    case SET_SECOND_ZERO_POINT:
    	action.driveTrain.speed.usingClosedLoop = true;
    	action.driveTrain.mode = action.driveTrain.POSITION;

    	action.driveTrain.speed.rawForward = 0.0;
    	action.driveTrain.speed.rawTurn = 0.0;

    	action.driveTrain.position.resetFwd = true;
    	action.driveTrain.position.resetTurn = true;
    	
    	action.driveTrain.distance.distanceSetPoint  = -6.0;
    	action.driveTrain.distance.bearingSetPoint 	= 0.0;
    	
    	timer = 0;
    	state = DRIVE_BACK;
    	break;
    	
    case DRIVE_BACK:
    	action.driveTrain.position.resetFwd = false;
    	action.driveTrain.position.resetTurn = false;
    	
    	action.driveTrain.position.maxFwdSpeed = 0.15;

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
//    	action.driveTrain.position.resetFwd  = true;
//    	action.driveTrain.position.resetTurn = true;
//    	first = false;
//    }
//    
//    action.driveTrain.mode = action.driveTrain.POSITION;
//    action.driveTrain.position.distanceSetPoint 	= 6*12.0;
//    action.driveTrain.position.bearingSetPoint 	= 0.0;
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
//    			action.driveTrain.position.resetFwd = true;
//    			action.driveTrain.position.resetTurn = true;
//    			
//    			action.driveTrain.position.distanceSetPoint  = -2*12.0;
//    			action.driveTrain.position.bearingSetPoint 	= 0.0;
//    			cycleCount = -1;
//    		}
//    	}
//    }

//    action.driveTrain.speed.rawForward = inputs.GetForward();
//    action.driveTrain.speed.rawTurn    = inputs.GetTurn();
//
//    action.driveTrain.speed.brakeLeft  = inputs.ShouldBrakeLeft();
//    action.driveTrain.speed.brakeRight = inputs.ShouldBrakeRight();

//    action.driveTrain.speed.thirdGear  = inputs.ShouldShiftThird();

}
