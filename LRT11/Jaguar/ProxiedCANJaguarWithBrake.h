#ifndef PROXIED_CAN_JAGUAR_WITH_BRAKE_H_
#define PROXIED_CAN_JAGUAR_WITH_BRAKE_H_

#include "..\General.h"
#include "ProxiedCANJaguar.h"
#include "CANJaguarBrake.h"

class ProxiedCANJaguarWithBrake : public ProxiedCANJaguar
{
private:
    CANJaguarBrake brake;

public:
    ProxiedCANJaguarWithBrake(UINT8 channel);
    void ApplyBrakes(int brakeAmount);
    void UpdateOutput();
};

#endif
