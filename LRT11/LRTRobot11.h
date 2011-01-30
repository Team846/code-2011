#ifndef LRT_ROBOT_11_H_
#define LRT_ROBOT_11_H_

#include "General.h"
#include "LRTRobotBase.h"
#include "Brain/Brain.h"
#include "Components/DriveTrain.h"
#include "Components/EncoderDataCollection.h"
#include "Config/Config.h"

class LRTRobot11 : public LRTRobotBase
{
private:
    Brain brain;
    DriveTrain drive;

    EncoderDataCollection encoderData;
    GameState DetermineState();

public:
    LRTRobot11();
    virtual ~LRTRobot11();

    virtual void RobotInit();
    virtual void MainLoop();
};

#endif
