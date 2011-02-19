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

    Config& config;
    enum {STOPPED = 1, SUCKING = 2, SPITTING = 3, ROLLING = 4};

    float currentSuckingIn, currentSpittingOut;

    void RollInward();
    void RollOutward();
    void Stop();
    void RollOpposite(int direction);
public:
    Roller();
    virtual ~Roller();

    virtual void Output();
    virtual void Configure();
};

#endif
