#ifndef MODIFIED_DRIVE_TRAIN_H_
#define MODIFIED_DRIVE_TRAIN_H_

#include "..\General.h"
#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "DriveTrain\DitheredBrakeTrain.h"
#include "..\Jaguar\Esc.h"

class ModifiedDriveTrain : public Component
{
private:
    DitheredBrakeTrain ditheredBrakeTrain;
    Esc leftESC, rightESC;

public:
    ModifiedDriveTrain();
    virtual ~ModifiedDriveTrain();

    virtual void Output();
};

#endif
