#ifndef LIFT_H_
#define LIFT_H_

#include "Component.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Config\Config.h"
#include "..\Sensors\VirtualPot.h"
#include "Lift\LiftPotReader.h"

class Lift : public Component, public Configurable
{
private:
    Config& config;
    string prefix;
    ProxiedCANJaguar liftEsc;
#ifdef VIRTUAL
    VirtualPot liftPot;
#endif

    LiftPotReader& potReader;
    const static float inchesToTurns = 1.0 / 12.0;

    int timeoutCycles;
    int cycleCount;

    float minPosition;
    float maxPosition;

    enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4};
    void StartTimer();

    enum {MANUAL = 1, PRESET = 2} prevMode;
    float potDeadband;
    bool positionMode;

public:
    Lift();
    virtual ~Lift();

    virtual void Configure();
    void ConfigureManualMode();

    virtual void Output();
};

#endif
