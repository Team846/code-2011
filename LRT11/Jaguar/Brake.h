#ifndef BRAKE_H_
#define BRAKE_H_

#include "..\General.h"

class Brake
{
public:
    Brake() { }

    virtual ~Brake()
    {
    }

    virtual void ApplyBrakes(int brakeAmount) = 0;
    virtual void BrakeFull() = 0;
    virtual void SetCoast() = 0;
};

#endif /* BRAKE_H_ */
