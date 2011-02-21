#ifndef LRT_ROBOT_11_H_
#define LRT_ROBOT_11_H_

#include "General.h"
#include "LRTRobotBase.h"
#include "Brain/Brain.h"
#include "Components/DriveTrain.h"
#include "Components/EncoderDataCollection.h"
#include "Components/Shifter.h"
#include "Components/PositionDriveTrain.h"
#include "Components/DemoLift.h"
#include "Config/Config.h"

class LRTRobot11 : public LRTRobotBase
{
private:
    Brain brain;
    CANBusController& controller;
    DriveTrain drive;
//    PositionDriveTrain positionDrive;

//    EncoderDataCollection encoderData;
//    Shifter shifter;

    DemoLift lift;
    Config& config;

    GameState prevState;
    GameState DetermineState();

public:
    LRTRobot11();
    virtual ~LRTRobot11();

    virtual void RobotInit();
    virtual void MainLoop();
};

#endif
