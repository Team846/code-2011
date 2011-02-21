#ifndef CL_POSITION_JAGUAR_H_
#define CL_POSITION_JAGUAR_H_

#include "..\General.h"
#include "ProxiedCANJaguar.h"
#include "..\Config\Config.h"

class CLPotJaguar : public ProxiedCANJaguar, public Configurable
{
private:
    string prefix;
    int turns;

public:
    CLPotJaguar(UINT8 channel, int potTurns, string configPrefix);
    ~CLPotJaguar();

    virtual void Configure();
};

#endif
