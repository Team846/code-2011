#ifndef CONFIGURABLE_H_
#define CONFIGURABLE_H_

#include "../General.h"

class Configurable
{
public:
    Configurable();
    virtual void Configure() = 0;
};

#endif
