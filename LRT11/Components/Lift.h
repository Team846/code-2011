#ifndef LIFT_H_
#define LIFT_H_

#include "Component.h"
#include "..\Jaguar\CLPotJaguar.h"
#include "..\Config\Config.h"

class Lift : public Component, public Configurable
{
private:
    Config& config;
    string prefix;
    ProxiedCANJaguar liftEsc;

    int timeoutMs;
    int cycleCount;

    enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4};

public:
    Lift();
    virtual ~Lift();

    virtual void Configure();
    virtual void Output();
};

#endif
