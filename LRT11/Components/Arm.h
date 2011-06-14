#ifndef ARM_H_
#define ARM_H_

#include "Component.h"
#include "..\Config\Configurable.h"
using namespace std;

class ProxiedCANJaguar;
class Config;
class VirtualPot;

class Arm : public Component, public Configurable
{
private:
    Config& config;
    string prefix;
    ProxiedCANJaguar* armEsc;
#ifdef VIRTUAL
    VirtualPot* armPot;
#else
    AnalogChannel* armPot;
#endif

    int oldState;

    float maxPosition, minPosition, midPosition, midPositionDeadband;
    float powerUp, powerRetainUp, powerDown, midPGain;
    float midPowerUp, midPowerDown;

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

    virtual string GetName();
};

#endif
