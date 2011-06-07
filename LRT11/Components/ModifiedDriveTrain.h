#ifndef MODIFIED_DRIVE_TRAIN_H_
#define MODIFIED_DRIVE_TRAIN_H_

#include "..\General.h"
#include "Component.h"
#include "..\Config\Config.h"
#include "..\Config\RobotConfig.h"
#include "DriveTrain\CLRateTrain.h"
#include "DriveTrain\CLPositionDriveTrain.h"
#include "..\Jaguar\Esc.h"

class ModifiedDriveTrain : public Component, public Configurable
{
private:
    DriveEncoders& driveEncoders;
    CLRateTrain closedRateTrain;
    CLPositionDriveTrain closedPositionTrain;
    Esc leftESC, rightESC;
    Config& config;

    int cyclesToSynchronize;  //value in config file
    int synchronizedCyclesLeft;


    float GetSynchronizedSpeed(float motorSpeed);
    virtual void Configure();

public:
    ModifiedDriveTrain();
    virtual ~ModifiedDriveTrain();

    virtual void Output();
};

#endif
