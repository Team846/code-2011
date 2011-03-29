#ifndef ARM_H_
#define ARM_H_

#include "Component.h"
#include "..\Config\Config.h"
#include "..\Jaguar\ProxiedCANJaguarWithBrake.h"
#include "..\Sensors\VirtualPot.h"
using namespace std;

class Arm : public Component, public Configurable
{
private:
    Config& config;
    string prefix;
    ProxiedCANJaguar armEsc;
#ifdef VIRTUAL
    VirtualPot armPot;
#else
    AnalogChannel armPot;
#endif

    float maxPosition, minPosition;
    float powerUp, powerRetainUp, powerDown;

    int timeoutCycles;
    int cycleCount;

    enum {BOTTOM = 1, TOP = 2};
    bool presetMode;

    int pulseCount;

    const static float ARM_UP_THRESHOLD = 10;

public:
    Arm();
    virtual ~Arm();

    virtual void Configure();
    virtual void Output();
};

#endif
