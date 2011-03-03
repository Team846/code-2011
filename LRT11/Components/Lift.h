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
    CANBusController& controller;

    const static float inchesToTurns = 1 / 12.0;

    int timeoutMs;
    int cycleCount;

    float minPosition;
    float maxPosition;

    enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4};
    void StartTimer();

    enum {MANUAL = 1, PRESET = 2} prevMode;
    float potDeadband;

public:
    Lift();
    virtual ~Lift();

    virtual void Configure();
    void ConfigureVoltageMode();

    virtual void Output();
};

#endif
