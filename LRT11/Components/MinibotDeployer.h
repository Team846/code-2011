#ifndef MINIBOT_DEPLOYER_H
#define MINIBOT_DEPLOYER_H

#include "Component.h"
#include "Shifter\LRTServo.h"
#include "Shifter\VIRTUALLRTServo.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Config\Config.h"

class MinibotDeployer : public Component
{
public:
    MinibotDeployer();
    virtual ~MinibotDeployer();

    virtual void Output();

private:
    Config& config;
    ProxiedCANJaguar deployerEsc;

    float lockedServoValue;
    float releasedServoValue;

    int currentThreshold;

#ifdef VIRTUAL
    VirtualLRTServo leftAlignerServo, rightAlignerServo;
#else
    LRTServo leftAlignerServo, rightAlignerServo;
#endif
};

#endif
