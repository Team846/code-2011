#include "DemoLift.h"
#include "..\Config\RobotConfig.h"

DemoLift::DemoLift()
    : liftEsc(RobotConfig::CAN_LIFT)
{

}

DemoLift::~DemoLift()
{

}

void DemoLift::Output()
{
    float forward = action.demoLift.forward;
    liftEsc.Set(forward);

    SmartDashboard::Log(forward, "Demo Lift Set Point");
}
