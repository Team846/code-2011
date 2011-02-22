#ifndef ARM_H_
#define ARM_H_

#include "Component.h"
#include "..\Config\Config.h"
#include "..\Jaguar\ProxiedCANJaguarWithBrake.h"
using namespace std;

class Arm : public Component, public Configurable
{
private:
    Config& config;
    string prefix;
    ProxiedCANJaguar armEsc;
    AnalogChannel armPot;

    float maxPosition, minPosition;
    float powerUp, powerDown;

    int timeoutMs;
    int cycleCount;

    enum {BOTTOM = 1, TOP = 2};

public:
    Arm();
    virtual ~Arm();

    virtual void Configure();
    virtual void Output();
};

#endif
