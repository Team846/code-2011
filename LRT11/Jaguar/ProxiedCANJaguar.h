#ifndef PROXIED_CAN_JAGUAR_H_
#define PROXIED_CAN_JAGUAR_H_

#include "..\General.h"
#include "..\CAN\CANBusController.h"
#include "..\Util\Util.h"

class ProxiedCANJaguar : public SpeedController
{
public:
    ProxiedCANJaguar(UINT8 channel);
    ~ProxiedCANJaguar();

    int GetChannel();
    virtual void Set(float setpoint, UINT8 syncGroup = 0);

    virtual float Get();
    virtual void Disable();

    void SetPID(double p, double i, double d);
    void SetPositionReference(CANJaguar::PositionReference reference);
    void SetPotentiometerTurns(UINT16 turns);
    CANJaguar::PositionReference GetPositionReference(void);

    void SetControlMode(CANJaguar::ControlMode controlMode);
    CANJaguar::ControlMode GetControlMode();
    void EnableControl(double encoderInitialPosition = 0.0);
    void DisableControl();

    float GetCurrent();
    float GetTemperature();
    float GetBatteryVoltage();
    float GetOutputVoltage();

    double GetPosition();
    double GetSpeed();

    void ConfigSoftPositionLimits(double forwardLimit, double reverseLimit);
    void DisableSoftPositionLimits();
    bool GetForwardLimitOK();
    bool GetReverseLimitOK();

    void ConfigNeutralMode(CANJaguar::NeutralMode mode);

protected:
    CANBusController& controller;
    int channel;
};

#endif /* PROXIED_CAN_JAGUAR_H_ */
