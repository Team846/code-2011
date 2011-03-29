#ifndef MODIFIED_DRIVE_TRAIN_H_
#define MODIFIED_DRIVE_TRAIN_H_

#include "..\General.h"
#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "DriveTrain\CLRateTrain.h"
#include "DriveTrain\CLPositionDriveTrain.h"
#include "..\Jaguar\Esc.h"

class ModifiedDriveTrain : public Component
{
private:
    CLRateTrain closedRateTrain;
    CLPositionDriveTrain closedLoopPositionTrain;
    Esc leftESC, rightESC;
    
    enum
    {
        SPEED,
        POSITION,
        DISTANCE
    } 

public:
    ModifiedDriveTrain();
    virtual ~ModifiedDriveTrain();

    virtual void Output();
};

#endif
