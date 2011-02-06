#include "Arm.h"

Arm::Arm()
: armEsc(13)

{
	
}

Arm::~Arm()
{
	
}

void Arm::ArmShiftUp()
{
//	m_ArmState = kArmRaising;
	armEsc.Set(1.0);
	
}

void Arm::ArmShiftDown()
{
//	m_ArmState = kArmLowering;
	armEsc.Set(-1.0);
}

void Arm::AbortArm()
{
//	m_ArmState = kArmIdle;
	armEsc.Set(0);
}
