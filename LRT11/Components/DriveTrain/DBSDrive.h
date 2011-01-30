#ifndef DBSDRIVE_H
#define DBSDRIVE_H

#include "..\..\General.h"
#include "..\..\Jaguar\Brake.h"
#include "..\..\Jaguar\Esc.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "DriveMethod.h"

class DBSDrive : public DriveMethod, public Configurable
{
protected:
    int maxBraking;
    bool isSquaredInputs;
    string prefix;
    float turnInPlaceThreshold;
    float brakeTurnDeadband;

public:
    DBSDrive(Esc& leftDrive, Esc& rightDrive, bool isSquaredInputs);
    virtual ~DBSDrive();

    virtual void Configure();
    void SetSquaredInputsEnabled(bool enabled);
    virtual DriveOutput ComputeArcadeDrive(float forward, float turn);
};

#endif /* DBSDRIVE_H */
