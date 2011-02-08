#include "Arm.h"
#include "..\Config\RobotConfig.h"

Arm::Arm()
    : config(Config::GetInstance())
    , prefix("arm.")
    , armEsc(RobotConfig::CAN_ARM)
{
	armEsc.SetControlMode(CANJaguar::kPosition);
	armEsc.SetPositionReference(CANJaguar::kPosRef_Potentiometer);
	armEsc.SetPID(config.Get<double>(prefix + "PGain"), config.Get<double>(prefix + "IGain"), config.Get<double>(prefix + "DGain"));
	armEsc.ConfigSoftPositionLimits(config.Get<float>(prefix + "max"),config.Get<float>(prefix + "min"));
	armEsc.EnableControl();
}

Arm::~Arm()
{

}

void Arm::Output()
{
	string key = prefix;
	float armSetPoint;
	
	if(action.arm.preset)
	{
		switch(action.arm.position)
		{
		case kLow:
			key += "low";
			break;
		case kHigh:
			key += "high";
			break;
		}
		armSetPoint = config.Get<float>(key);
	}
	else
	{
		armSetPoint = action.arm.customSetpoint;
	}
	
	armEsc.Set(armSetPoint);
	
	SmartDashboard::Log(armSetPoint, "ArmSetPoint");
	SmartDashboard::Log(armEsc.GetPosition(), "ArmPosition");
}
