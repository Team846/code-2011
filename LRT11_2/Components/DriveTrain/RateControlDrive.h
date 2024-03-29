#ifndef CL_RATE_TRAIN_H_
#define CL_RATE_TRAIN_H_

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Jaguar\Esc.h"
#include "..\..\Util\RunningSum.h"
#include "DBSDrive.h"

class RateControlDrive : public DBSDrive
{
public:
    RateControlDrive();

    virtual void Configure();
    DriveCommand Drive(float rawFwd, float rawTurn);

    void PivotLeft(float rightSpeed);
    void PivotRight(float leftSpeed);

    void SetBrakeLeft(bool brakeLeft);
    void SetBrakeRight(bool brakeRight);

    void SetClosedLoopEnabled(bool enabled);
    void SetHighGear(bool isHighGear);

    void Stop();
    
protected:
    DriveEncoders& encoders;
    Config& config;
    bool IsHighGear();
    bool UsingClosedLoop();

private:
//    Esc& escLeft, &escRight;

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

    const static float FWD_DECAY = 0.87;
    const static float TURN_DECAY = 0.87; // (1/2)^(1/5) =~ 0.87
};

#endif
