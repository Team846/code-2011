#ifndef DEMO_LIFT_H_
#define DEMO_LIFT_H_

#include "Component.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Config\Config.h"

class DemoLift : public Component
{
private:
    ProxiedCANJaguar liftEsc;

public:
    DemoLift();
    virtual ~DemoLift();

    virtual void Output();
};

#endif
