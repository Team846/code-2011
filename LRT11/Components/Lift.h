#ifndef LIFT_H_
#define LIFT_H_

#include "Component.h"
#include "..\Jaguar\CLPotJaguar.h"
#include "..\Config\Config.h"

class Lift : public Component
{
private:
    Config& config;
    string prefix;
    CLPotJaguar liftEsc;

    enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4};

public:
    Lift();
    virtual ~Lift();

    virtual void Output();
};
#endif
