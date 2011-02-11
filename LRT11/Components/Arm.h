#ifndef ARM_H_
#define Arm_H_

#include "Component.h"
#include "..\Config\Config.h"
#include "..\Jaguar\CLPotJaguar.h"
using namespace std;

class Arm : public Component
{
private:
    Config& config;
    string prefix;
    CLPotJaguar armEsc;

    enum {kLow = 1, kHigh = 2} position;

public:
    Arm();
    virtual ~Arm();
    virtual void Output();
};

#endif
