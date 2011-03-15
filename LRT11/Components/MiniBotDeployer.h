#ifndef MINIBOT_DEPLOYER_H
#define MINIBOT_DEPLOYER_H

#include "Component.h"
#include "Shifter\LRTServo.h"
#include "Shifter\VIRTUALLRTServo.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Config\Config.h"

class MiniBotDeployer : public Component
{
public:
    MiniBotDeployer();
    virtual ~MiniBotDeployer();

    virtual void Output();

private:

    float left_locked, left_open;
    Config& config;
    int runCount, deployedCurrent;
    float lastSetPoint;


#ifdef VIRTUAL
    VirtualLRTServo leftAlignerServo, rightAlignerServo;
#else
    LRTServo leftAlignerServo, rightAlignerServo;
#endif

    ProxiedCANJaguar deployerESC;
};

#endif
