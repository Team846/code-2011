#ifndef MODIFIED_DRIVE_TRAIN_H_
#define MODIFIED_DRIVE_TRAIN_H_

#include "..\General.h"
#include "Component.h"
#include "..\Config\Configurable.h"

class Config;
class CLPositionDriveTrain;
class RateControlDrive;
class Esc;
class DriveEncoders;
class SynchronizedDrive;

class ModifiedDriveTrain : public Component, public Configurable
{
private:
    DriveEncoders& driveEncoders;
    
    RateControlDrive* closedRateTrain;
    CLPositionDriveTrain* closedPositionTrain;
    SynchronizedDrive* synchronizedDrive;
    
    Esc* leftESC, *rightESC;
    Config& config;

    int cyclesToSynchronize;  //value in config file
    int synchronizedCyclesLeft;


    virtual void Configure();

public:
    ModifiedDriveTrain();
    virtual ~ModifiedDriveTrain();

    virtual void Output();
    virtual string GetName();
};

#endif
