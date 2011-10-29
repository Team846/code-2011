#ifndef DBS_DRIVE
#define DBS_DRIVE

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"

typedef struct
{
    float dutyCycle;
    float brakingDutyCycle;
} ESCCommand;

typedef struct
{
    ESCCommand leftCommand;
    ESCCommand rightCommand;
    bool shouldBrake;
} DriveCommand;

class DBSDrive : public Configurable
{
private:
    LRTEncoder& leftEncoder;
    LRTEncoder& rightEncoder;
    DriveEncoders& driveEncoders;

public:
    DBSDrive();
    ~DBSDrive();

    virtual void Configure();
    DriveCommand Drive(float forwardInput, float turnInput);
    ESCCommand CalculateBrakeAndDutyCycle(float input, float speed);
};

#endif
