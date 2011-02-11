#ifndef POSITION_DRIVE_TRAIN_H_
#define POSITION_DRIVE_TRAIN_H_

#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "..\Jaguar\Esc.h"
#include "..\Sensors\DriveEncoders.h"
#include "DriveTrain\DBSDrive.h"
#include "DriveTrain\ClosedLoopDriveTrain.h"
#include "DriveTrain\CLPositionDriveTrain.h"

class PositionDriveTrain : public Component
{
private:
    DriveEncoders& encoders;
    Esc left, right;
    DBSDrive dbsDrive;
    ClosedLoopDriveTrain robotDrive;
    CLPositionDriveTrain positionDrive;

public:
    PositionDriveTrain();
    virtual ~PositionDriveTrain();

    virtual void Output();
};

#endif
