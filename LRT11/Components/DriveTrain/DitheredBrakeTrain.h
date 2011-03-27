#ifndef DITHERED_BRAKE_DRIVE
#define DITHERED_BRAKE_DRIVE

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"

typedef struct
{
    float leftDC;
    float rightDC;

    float leftBrakeDC;
    float rightBrakeDC;
} DriveCommand;

class DitheredBrakeTrain : public Configurable
{
private:
    LRTEncoder& leftEncoder;
    LRTEncoder& rightEncoder;

    float brakeGain;

public:
    DitheredBrakeTrain();
    ~DitheredBrakeTrain();

    virtual void Configure();
    DriveCommand Drive(float forwardInput, float turnInput);
};

#endif
