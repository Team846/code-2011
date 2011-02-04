#include "Roller.h"

Roller::Roller()
	: topRoller(10) // change port number later
	, bottomRoller(11)
{
	
}

Roller::~Roller()
{
	
}

void Roller::RollInward() 
{
	topRoller.Set(1.0);
	bottomRoller.Set(1.0);
}

void Roller::RollOutward()
{
	topRoller.Set(-1.0);
	bottomRoller.Set(-1.0);
}

void Roller::RollOpposite(int direction)
{
	topRoller.Set(Util::Sign<int>(direction) * 1.0);
	bottomRoller.Set(Util::Sign<int>(direction) * -1.0);
}

void Roller::Output()
{
	// to be defined later
}
