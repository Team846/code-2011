#ifndef PROXIED_CAN_JAGUAR_H_
#define PROXIED_CAN_JAGUAR_H_

#include "..\General.h"
#include "..\CAN\CANBusController.h"
#include "..\Util\Util.h"

class ProxiedCANJaguar : public SpeedController
{
public:
    ProxiedCANJaguar(UINT8 channel);

    virtual void Set(float speed, UINT8 syncGroup = 0);
    virtual float Get();
    virtual void Disable();

    float GetCurrent();
    float GetTemperature();
    float GetBatteryVoltage();
    float GetOutputVoltage();

    void ConfigNeutralMode(CANJaguar::NeutralMode mode);

protected:
    CANBusController& controller;
    int channel;
};

#endif /* PROXIED_CAN_JAGUAR_H_ */
