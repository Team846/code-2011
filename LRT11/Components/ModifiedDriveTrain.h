#ifndef MODIFIED_DRIVE_TRAIN_H_
#define MODIFIED_DRIVE_TRAIN_H_

#include "..\General.h"
#include "Component.h"
#include "..\Config\Config.h"
#include "..\Config\RobotConfig.h"
#include "DriveTrain\CLRateTrain.h"
#include "DriveTrain\CLPositionDriveTrain.h"
#include "..\Jaguar\Esc.h"

class ModifiedDriveTrain : public Component
{
private:
    DriveEncoders& driveEncoders;
    CLRateTrain closedRateTrain;
    CLPositionDriveTrain closedPositionTrain;
    Esc leftESC, rightESC;
    Config& config;

    int cyclesToSynchronize;  //value in config file
    int synchronizedCyclesLeft;
    enum
    {
        SPEED,
        POSITION,
        DISTANCE,
        SYNCHRONIZING
    };

    float GetSynchronizedSpeed(float motorSpeed);

public:
    ModifiedDriveTrain();
    virtual ~ModifiedDriveTrain();

    virtual void Output();
};

#endif
