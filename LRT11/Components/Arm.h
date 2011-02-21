#ifndef ARM_H_
#define ARM_H_

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

    enum {BOTTOM = 1, TOP = 2};

public:
    Arm();
    virtual ~Arm();
    virtual void Output();
};

#endif
