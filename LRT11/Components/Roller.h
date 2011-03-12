#ifndef ROLLER_H_
#define ROLLER_H_

#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "..\Config\Config.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Util\Util.h"

class Roller : public Component, public Configurable
{
private:
    ProxiedCANJaguar topRoller;
    ProxiedCANJaguar bottomRoller;

    string prefix;
    enum {STOPPED = 1, SUCKING = 2, SPITTING = 3, ROTATING = 4};

    float dutyCycleSucking;

    float dutyCycleSpittingTop;
    float dutyCycleSpittingBottom;

    float dutyCycleRotatingIn;
    float dutyCycleRotatingOut;

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
