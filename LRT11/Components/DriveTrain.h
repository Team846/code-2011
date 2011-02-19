#ifndef DRIVE_TRAIN_H_
#define DRIVE_TRAIN_H_

#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "..\Jaguar\Esc.h"
#include "..\Sensors\DriveEncoders.h"
#include "DriveTrain\CLRateDriveTrain.h"
#include "DriveTrain\DBSDrive.h"

class DriveTrain : public Component
{
private:
    DriveEncoders& encoders;
    Esc left, right;
    DBSDrive dbsDrive;
    CLRateDriveTrain robotDrive;

public:
    DriveTrain();
    virtual ~DriveTrain();

    virtual void Output();
};

#endif
