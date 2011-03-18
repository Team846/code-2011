#ifndef CLOSED_LOOP_DRIVE_TRAIN_H_
#define CLOSED_LOOP_DRIVE_TRAIN_H_

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Jaguar\Esc.h"
#include "..\..\Util\RunningSum.h"
#include "DriveMethod.h"
#include "DBSDrive.h"

class CLRateDriveTrain : public DriveMethod, public Configurable
{
public:
    CLRateDriveTrain(Esc& escLeft, Esc& escRight, DriveEncoders& encoders,
            DBSDrive& dbsDrive);

    virtual DriveOutput ComputeArcadeDrive(float rawFwd, float rawTurn);
    void PivotLeft(float rightSpeed);
    void PivotRight(float leftSpeed);

    void SetBrakeLeft(bool brakeLeft);
    void SetBrakeRight(bool brakeRight);

    void SetClosedLoopEnabled(bool enabled);

    void Stop();
    virtual void Configure();

    void SetHighGear(bool isHighGear);

private:
    Esc& escLeft, &escRight;
    DriveEncoders& encoders;

    Config& config;
    DBSDrive& dbsDrive;

    float pGainTurnLowGear;
    float pGainFwdLowGear;

    float pGainTurnHighGear;
    float pGainFwdHighGear;

    RunningSum fwdRunningError;
    RunningSum turnRunningError;

    bool brakeLeft;
    bool brakeRight;

    bool usingClosedLoop;

    bool highGear;

    const static float FWD_DECAY = 0.5;
    const static float TURN_DECAY = 0.5;
};

#endif
