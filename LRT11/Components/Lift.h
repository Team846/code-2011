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

    const static float inchesToTurns = 1 / 12.0;

    int timeoutMs;
    int cycleCount;

    float safetyDeadbandInches;
    float safetyMarginInches;

//    bool safety;

    enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4};
    void StartTimer();

public:
    Lift();
    virtual ~Lift();

    virtual void Configure();
    virtual void Output();
};

#endif
