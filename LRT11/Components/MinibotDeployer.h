#ifndef MINIBOT_DEPLOYER_H
#define MINIBOT_DEPLOYER_H

#include "Component.h"
#include "Shifter\LRTServo.h"
#include "Shifter\VIRTUALLRTServo.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Config\Config.h"

class MinibotDeployer : public Component, public Configurable
{
public:
    MinibotDeployer();
    virtual ~MinibotDeployer();

    virtual void Output();
    virtual void Configure();

private:
    Config& config;
    ProxiedCANJaguar deployerEsc;

    float lockedServoValue;
    float releasedServoValue;

    int currentThreshold;

#ifdef VIRTUAL
    VirtualLRTServo alignerServo;
#else
    LRTServo alignerServo;
#endif
};

#endif
