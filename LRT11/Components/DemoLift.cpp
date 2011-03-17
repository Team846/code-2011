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
    float forward = action.demoLift.power;
    liftEsc.Set(forward);

#ifdef USE_DASHBOARD
//    SmartDashboard::Log(forward, "Demo Lift Set Point");
#endif
}
