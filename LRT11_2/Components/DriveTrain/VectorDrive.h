#ifndef CL_RATE_TRAIN_H_
#define CL_RATE_TRAIN_H_

#include "..\..\General.h"

#include "..\..\Util\RunningSum.h"

#include "RateControlDrive.h"
#include "DBSDrive.h"

//class VectorDrive : public RateControlDrive
{
public:
    VectorDrive();

    virtual void Configure();
    DriveCommand Drive(float heading, float fwd);

    void Stop();
private:
    float pGainHeadingLowGear;
    float pGainHeadingHighGear;

    RunningSum headingRunningError;

    const static float TURN_DECAY = 0.87; // (1/2)^(1/5) =~ 0.87
};

#endif
