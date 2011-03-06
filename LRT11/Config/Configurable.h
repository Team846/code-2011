#ifndef CONFIGURABLE_H_
#define CONFIGURABLE_H_

#include "../General.h"
#include "../Util/AsynchronousPrinter.h"

class Configurable
{
public:
    Configurable();
    virtual ~Configurable();

    virtual void Configure() = 0;
};

#endif
