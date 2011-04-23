#ifndef PROXIED_CAN_JAGUAR_H_
#define PROXIED_CAN_JAGUAR_H_

#include "..\General.h"
#include "..\CAN\VirtualCANBusController.h"
#include "..\Util\Util.h"

class ProxiedCANJaguar : public CANJaguar
{
private:
    float lastSetpoint;
    int channel;

    GameState lastState;
    static GameState gameState;

    int index;

public:
    ProxiedCANJaguar(UINT8 channel);
    ~ProxiedCANJaguar();

    typedef struct
    {
        int num;
        ProxiedCANJaguar* j[32];

        float currents[32];
        float potValues[32];

        bool shouldCollectCurrent[32];
        bool shouldCollectPotValue[32];
    } JaguarList;

    static JaguarList jaguars;
    virtual void Set(float setpoint, UINT8 syncGroup = 0);

    void CollectCurrent();
    void CollectPotValue();

    float GetCurrent();

#ifdef VIRTUAL
    virtual float Get();
    virtual void Disable();

    void SetPID(double p, double i, double d);
    void SetPositionReference(VirtualPot* reference);
    void ConfigPotentiometerTurns(UINT16 turns);
    CANJaguar::PositionReference GetPositionReference(void);

    void ChangeControlMode(CANJaguar::ControlMode controlMode);
    CANJaguar::ControlMode GetControlMode();
    void EnableControl(double encoderInitialPosition = 0.0);
    void DisableControl();

    float GetCurrent();
    float GetTemperature();
    float GetBatteryVoltage();
    float GetOutputVoltage();

    double GetPosition();
    double GetSpeed();

    void ConfigNeutralMode(CANJaguar::NeutralMode mode);
    void ResetCache();
#else
    static void SetGameState(GameState state);
    void ResetCache();
#endif

protected:
#ifdef VIRTUAL
    VirtualCANBusController& controller;
#else
//    CANBusController& controller;
#endif
};

#endif /* PROXIED_CAN_JAGUAR_H_ */
