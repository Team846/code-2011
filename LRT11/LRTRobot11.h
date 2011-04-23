#ifndef LRT_ROBOT_11_H_
#define LRT_ROBOT_11_H_

#include "General.h"
#include "LRTRobotBase.h"
#include "Brain/Brain.h"
#include "Components/ModifiedDriveTrain.h"
#include "Components/EncoderDataCollection.h"
#include "Components/Shifter.h"
#include "Components/PositionDriveTrain.h"
#include "Components/Lift.h"
#include "Components/Arm.h"
#include "Components/Roller.h"
#include "Components/MiniBotDeployer.h"
#include "Components/CANBusTester.h"
#include "Config/Config.h"
#include "wdLib.h"
#include "sysLib.h"

class LRTRobot11 : public LRTRobotBase
{
private:
    Brain brain;
#ifdef VIRTUAL
    VirtualCANBusController& controller;
#else
//    CANBusController& controller;
#endif

    ModifiedDriveTrain drive;
//    PositionDriveTrain positionDrive;

    Shifter shifter;

    Lift lift;
    Arm arm;
    Roller roller;

    MinibotDeployer minibotDeployer;
    Config& config;

    DriverStation& ds;
//    DigitalOutput switchLED;

    GameState prevState;
    GameState DetermineState();

//    AnalogChannel armPot;

    WDOG_ID mainLoopWatchDog;
public:
    LRTRobot11();
    virtual ~LRTRobot11();

    virtual void RobotInit();
    virtual void MainLoop();
};

#endif
