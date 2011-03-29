#include "CLPositionDriveTrain.h"
#include <math.h>
CLPositionDriveTrain::CLPositionDriveTrain(CLRateTrain& train)
    : drive(train)
    , encoders(DriveEncoders::GetInstance())
{
}

void CLPositionDriveTrain::Configure()
{
    Config& config = Config::GetInstance();
    const string prefix = "CLPositionDriveTrain.";

    pGainFwd = config.Get<float>(prefix + "pGainFwd", 1.5);
    pGainFwdTurnCorrection = config.Get<float>(prefix + "pGainFwdTurnCorrection", 0.01);
    pGainTurn = config.Get<float>(prefix + "pGainTurn", 1.5);
    pGainTurnFwdCorrection = config.Get<float>(prefix + "pGainTurnFwdCorrection", 0.02);

    fwdDeadband = config.Get<float>(prefix + "fwdDeadband", 0.05);
    turnDeadband = config.Get<float>(prefix + "turnDeadband", 0.05);
}

DriveCommand CLPositionDriveTrain::Drive(float fwdSetPoint, float turnSetpoint, bool stop)
{
    bool done;

    if(fabs(fwdSetPoint) < 1e-2)
        fwdSetPoint = 0.0;
    if(fabs(turnSetpoint) < 1e-2)
        turnSetpoint = 0.0;

    float fwdCorrection = 0.0, turnCorrection = 0.0;
    if(turnSetpoint == 0.0 && fwdSetPoint == 0.0)
    {
        //don't do anything
    }
    else if(turnSetpoint == 0.0) // no turning
    {
        float currentDist = encoders.GetRobotDist() - zeroDistance;
        float error = fwdSetPoint - currentDist;
        fwdCorrection = error * pGainFwd; //allow robot to run at max speed

        float turnError = zeroBearing - encoders.GetTurnTicks();
        turnCorrection = turnError * pGainFwdTurnCorrection;
        done = fabs(error) < fwdDeadband;
    }

    //turn with no fwd motion, not sure it works with pivots
    else if(fwdSetPoint == 0.0)
    {
        float currentBearing = encoders.GetTurnAngle() - zeroBearing;
        float error = turnSetpoint - currentBearing;
        turnCorrection = error * pGainTurn;

        float fwdError = zeroDistance - encoders.GetRobotDist();
        fwdCorrection = pGainTurnFwdCorrection * fwdError;

        done = fabs(error) < turnDeadband;
    }
    
    return drive.Drive(fwdCorrection , turnCorrection);
}

CLPositionCommand CLPositionDriveTrain::DriveAtLeastDistance(float dutyCycle)
{
	CLPositionCommand outputs; 
	
	dutyCycle = fabs(dutyCycle);//duty cycle is always >=0
	
	float fwdCorrection = 0, turnCorrection = 0;
	if (moveDistanceInfo.isSetUp)
	{
		//turn correction
        float turnError = moveDistanceInfo.initialBearing - encoders.GetTurnTicks();
        turnCorrection = turnError * pGainFwdTurnCorrection;
        
        //fwd pos
        if (moveDistanceInfo.goingForward)
        	fwdCorrection = dutyCycle;
        else
        	fwdCorrection = -dutyCycle;
        
        float error = moveDistanceInfo.target - encoders.GetRobotDist();
    	if (moveDistanceInfo.goingForward)
    		outputs.isDone = error <= 0;
    	else
    		outputs.isDone = error >= 0;
	}
	outputs.drive = drive.Drive(fwdCorrection, turnCorrection);
	return outputs;
}

void CLPositionDriveTrain::ResetFwd()
{
    zeroDistance = encoders.GetRobotDist();
}

void CLPositionDriveTrain::ResetTurn()
{
	// TODO Fix
    zeroBearing = encoders.GetTurnTicks();
}



void CLPositionDriveTrain::MoveInches(float inches)
{
    moveDistanceInfo.target 		= encoders.GetRobotDist() + inches;
    moveDistanceInfo.initialBearing = encoders.GetTurnTicks();
    moveDistanceInfo.goingForward   = inches > 0;
}
/*

void CLPositionDriveTrain::TurnAngle(float angle, bool pivotLeft, bool pivotRight)
{
    turnAngleInfo.target = encoders.GetTurnAngle() - angle;
    turnAngleInfo.distance = encoders.GetRobotDist();
    turnAngleInfo.pivotLeft = pivotLeft;
    turnAngleInfo.pivotRight = pivotRight;
    turnAngleInfo.hasCommand = true;
}

bool CLPositionDriveTrain::MoveDistanceOutput()
{
    if(!moveDistanceInfo.hasCommand)
        return true;

    drive.SetClosedLoopEnabled(true);
    drive.SetHighGear(false);

    float error = (moveDistanceInfo.target - encoders.GetRobotDist());
//    float newError = forwardRunningError.UpdateSum(error);

    // arcade drive assumes inputs are within [-1,1] interval
    // limit max speed to 50%
    float correction = Util::Clamp<float>(error * pGainFwd, -0.1, 0.1);

    float turnError = moveDistanceInfo.delta - encoders.GetTurnTicks();
    float turnCorrection = turnError * pGainFwdTurnCorrection;

    AsynchronousPrinter::Printf("E: %.2f, C: %.2f, TE: %.2f, TC: %.2f\n", error,
            correction, turnError, turnCorrection);
//    drive.ArcadeDrive(correction, turnCorrection);
    drive.ArcadeDrive(0.1, 0);

#ifdef USE_DASHBOARD
//    SmartDashboard::Log(newError, "CLPosition Error");
//    SmartDashboard::Log(correction, "CLPosition Correction");
#endif

    return Util::Abs<float>(error) < fwdDeadband;
}

bool CLPositionDriveTrain::TurnAngleOutput()
{
    if(!turnAngleInfo.hasCommand)
        return true;

    float error = (turnAngleInfo.target - encoders.GetTurnAngle());
    float newError = turnRunningError.UpdateSum(error);

    // arcade drive, pivot left, and pivot right all assume inputs are within [-1,1] interval
    float correction = Util::Clamp<float>(newError * pGainTurn, -1, 1);

    // output based on pivot flags
    if(turnAngleInfo.pivotLeft)
        drive.PivotLeft(correction);
    else if(turnAngleInfo.pivotRight)
        drive.PivotRight(correction);
    else
    {
        float fwdError = turnAngleInfo.distance - encoders.GetRobotDist();
        float fwdCorrection = pGainTurnFwdCorrection * fwdError;

        drive.ArcadeDrive(fwdCorrection, correction);
    }

#ifdef USE_DASHBOARD
//    SmartDashboard::Log(newError, "Turning error (position)");
//    SmartDashboard::Log(correction, "Turning correction (position)");
#endif

    return Util::Abs<float>(error) < turnDeadband;
}

void CLPositionDriveTrain::Stop()
{
    drive.Stop();
}

void CLPositionDriveTrain::SetClosedLoopEnabled(bool enabled)
{
    drive.SetClosedLoopEnabled(enabled);
}
*/
