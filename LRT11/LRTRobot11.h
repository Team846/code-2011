#ifndef LRT_ROBOT_11_H_
#define LRT_ROBOT_11_H_

#include "General.h"
#include "LRTRobotBase.h"
#include "Brain/Brain.h"
#include "Components/DriveTrain.h"
#include "Components/EncoderDataCollection.h"
#include "Components/Shifter.h"
#include "Components/PositionDriveTrain.h"
#include "Components/Lift.h"
#include "Components/Arm.h"
#include "Components/Roller.h"
#include "Components/MiniBotDeployer.h"
#include "Config/Config.h"

class LRTRobot11 : public LRTRobotBase
{
private:
    Brain brain;
#ifdef VIRTUAL
    VirtualCANBusController& controller;
#else
//    CANBusController& controller;
#endif

    DriveTrain drive;
//    PositionDriveTrain positionDrive;

    Shifter shifter;

    Lift lift;
    Arm arm;
    Roller roller;

    MinibotDeployer minibotDeployer;
    Config& config;

    GameState prevState;
    GameState DetermineState();

//    AnalogChannel armPot;

public:
    LRTRobot11();
    virtual ~LRTRobot11();

    virtual void RobotInit();
    virtual void MainLoop();
};

#endif
