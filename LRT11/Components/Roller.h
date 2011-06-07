#ifndef ROLLER_H_
#define ROLLER_H_

#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "..\Config\Config.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Util\Util.h"
#include "..\Jaguar\LRTCANJaguar.h"

class Roller : public Component, public Configurable
{
private:
    ProxiedCANJaguar topRoller;
    ProxiedCANJaguar bottomRoller;

    string prefix;

    float dutyCycleSucking;

    float dutyCycleSpittingTop;
    float dutyCycleSpittingBottom;

    float dutyCycleRotatingIn;
    float dutyCycleRotatingOut;

    int ignoreCycles;
    bool detected;

    void RollInward();
    void RollOutward();
    void RollOpposite(bool rotateUpward);
    void Stop();

public:
    Roller();
    virtual ~Roller();

    virtual void Output();
    virtual void Configure();
};

#endif
