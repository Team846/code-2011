#ifndef CL_POSITION_DRIVE_TRAIN_H_
#define CL_POSITION_DRIVE_TRAIN_H_

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Util\RunningSum.h"
#include "CLRateDriveTrain.h"

class CLPositionDriveTrain : public Configurable
{
public:
    CLPositionDriveTrain(CLRateDriveTrain& train);

    virtual void Configure();

    bool Output(float fwdSetPoint, float turnSetpoint);
    void ResetFwd();
    void ResetTurn();
private:
    CLRateDriveTrain& drive;
    DriveEncoders& encoders;

    float pGainFwd;
    float pGainFwdTurnCorrection;
    float pGainTurn;
    float pGainTurnFwdCorrection;

    float fwdDeadband;
    float turnDeadband;

    float zeroDistance, zeroBearing;
};

#endif
