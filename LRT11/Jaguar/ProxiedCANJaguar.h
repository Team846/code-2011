#ifndef PROXIED_CAN_JAGUAR_H_
#define PROXIED_CAN_JAGUAR_H_

#include "..\General.h"
#include "..\CAN\VirtualCANBusController.h"
#include "..\Util\Util.h"
#include "..\Util\StartOfCycleSubscriber.h"
#include "taskLib.h"

#define NON_BLOCKING

class ProxiedCANJaguar : public CANJaguar, public StartOfCycleSubscriber
{
private:
    float lastSetpoint;
    int channel;

    GameState lastState;
    static GameState gameState;

    int index;

#ifdef NON_BLOCKING
    static int setThreadEntryPoint(UINT32 proxiedCANJaguarPointer);
    int setThread();

    Task writerTask;
    SEM_ID setSemaphore;
    volatile bool shouldSetSetPoint;
    volatile float setPoint;
    volatile CANJaguar::NeutralMode neutralMode;
    volatile CANJaguar::NeutralMode lastNeutralMode;
    volatile bool shouldSetNeutralMode;
#endif
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
    void ConfigNeutralMode(CANJaguar::NeutralMode neutralMode);

    void CollectCurrent();
    void CollectPotValue();

    float GetCurrent();

    virtual void NewCycle();
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
