#ifndef CL_POSITION_JAGUAR_H_
#define CL_POSITION_JAGUAR_H_

#include "..\General.h"
#include "ProxiedCANJaguar.h"
#include "..\Config\Config.h"

class CLPotJaguar : public ProxiedCANJaguar, public Configurable
{
private:
    string prefix;

public:
    CLPotJaguar(UINT8 channel, int turns, string configPrefix, double defaultP = 1,
            double defaultI = 0, double defaultD = 0);
    ~CLPotJaguar();

    virtual void Configure();
};

#endif
