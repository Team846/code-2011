#ifndef SYNCHRONIZED_DRIVE
#define SYNCHRONIZED_DRIVE

#include "..\..\General.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "DBSDrive.h"


class SynchronizedDrive : public Configurable
{
private:
    LRTEncoder& leftEncoder;
    LRTEncoder& rightEncoder;
    DriveEncoders& driveEncoders;

    float EnsureFloorSpeed(float motorSpeed);
public:
    SynchronizedDrive();
    ~SynchronizedDrive();

    virtual void Configure();
    DriveCommand Drive();
};

#endif
