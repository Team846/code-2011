#include "vectorDrive.h"

VectorDrive::VectorDrive()
: headingRunningError(TURN_DECAY)
{
	SetCurrentHeadingAsZero();
}


void VectorDrive::Configure()
{
	pGainHeadingHighGear = config.Get<float>("VectorDrive", "pGainHeadingHighGear", 0.1);
	pGainHeadingLowGear = config.Get<float>("VectorDrive", "pGainHeadingLowGear", 0.1);
}

int VectorDrive::normalizeHeading(int heading)
{
	heading %= (int) (encoders.TICKS_PER_FULL_TURN);
	
	if (heading < 0)
		return heading + (int) encoders.TICKS_PER_FULL_TURN;
	else 
		return heading;
}
void VectorDrive::SetCurrentHeadingAsZero()
{
	zeroHeading = encoders.TurnTicks() % (int) (encoders.TICKS_PER_FULL_TURN);
	if (zeroHeading < 0)
		zeroHeading += encoders.TICKS_PER_FULL_TURN;
}

DriveCommand VectorDrive::Drive(float heading, float fwd)
{
	float currentHeading = normalizeHeading(encoders.TurnTicks() );
	currentHeading -= zeroHeading;
	
	float error = heading - currentHeading;
	error = headingRunningError.UpdateSum(error);
	
	float correction = error * (IsHighGear()?pGainHeadingHighGear:pGainHeadingLowGear);
	
	
	return RateControlDrive::Drive(fwd, correction);
}
