#ifndef LIFT_H_
#define LIFT_H_

#include "Component.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Config\Config.h"

class Lift : public Component
{
private:
    Config& config;
    string prefix;
    ProxiedCANJaguar liftEsc;

    enum {kStowed = 1, kLowPeg = 2, kMedPeg = 3, kHighPeg = 4};
public:
    Lift();
    virtual ~Lift();

    virtual void Output();
};
#endif
