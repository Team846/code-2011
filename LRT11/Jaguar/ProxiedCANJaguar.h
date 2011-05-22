#ifndef PROXIED_CAN_JAGUAR_H_
#define PROXIED_CAN_JAGUAR_H_

#include "..\General.h"
#include "..\CAN\VirtualCANBusController.h"
#include "..\Util\Util.h"
#include "LRTCANJaguar.h"
#include "taskLib.h"

class ProxiedCANJaguar : public LRTCANJaguar
{
private:
    int channel;
    char* name_;

    volatile float setpoint;
    volatile float lastSetpoint;

    volatile bool shouldCacheSetpoint;
    int cacheSetpointCounter;

    volatile NeutralMode mode;
    volatile NeutralMode lastMode;
    volatile bool shouldCacheMode;

    volatile float current;
    volatile bool collectCurrent;

    volatile float potValue;
    volatile bool collectPotValue;

    static GameState gameState;
    GameState lastState;

    int index;
    Task commTask;

    semaphore* commSemaphore;
    bool running_; //implementation in progress - controlled termination of task -dg
    bool quitting_; // ditto

public:
    ProxiedCANJaguar(UINT8 channel, char* name);
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
    void Set(float setpoint, UINT8 syncGroup = 0);
    void ConfigNeutralMode(LRTCANJaguar::NeutralMode mode);

    void ShouldCollectCurrent(bool shouldCollect);
    void ShouldCollectPotValue(bool shouldCollect);

    float GetCurrent();
    float GetPotValue();

    static int CommTaskWrapper(UINT32 proxiedCANJaguarPointer);
    void CommTask();

    void BeginComm();

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
