#ifndef CAN_JAGUAR_BRAKE_H_
#define CAN_JAGUAR_BRAKE_H_

#include "..\General.h"
#include "Brake.h"
#include "CANJaguar.h"
#include "ProxiedCANJaguar.h"
#include "..\Util\Util.h"

class CANJaguarBrake : public Brake
{
    ProxiedCANJaguar& jaguar;
    int cycleCount;
    int amount;

public:
    CANJaguarBrake(ProxiedCANJaguar& jaggie);
    virtual void ApplyBrakes(int brakeAmount);   // between 0 and 8

    virtual void BrakeFull()
    {
        ApplyBrakes(8);
    }

    virtual void SetCoast()
    {
        ApplyBrakes(0);
    }

    virtual void UpdateOutput();
};

#endif /* CAN_JAGUAR_BRAKE_H_ */
