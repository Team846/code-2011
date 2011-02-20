#ifndef LRT_CANBUSCTRLR_H_
#define LRT_CANBUSCTRLR_H_

#include "..\General.h"
#include "..\Jaguar\SafeCANJaguar.h"
#include "..\Util\AsynchronousPrinter.h"

//this macro defines a wrapper to the CANJag function mentioned in the macro argument.
//It does not work with functions that have parameters. The reason I chose to use a macro here
//is because this is used a lot and I wanted to avoid the extra 60+ lines of code and possibly introducing error
//WARNING: These functions are blocking
#define BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(a) a(int id)\
    {                                   \
        int idx = BusIdToIndex(id);     \
        return jaguars[idx]->a();       \
    }

class CANBusController : public SensorBase
{
public:
    virtual ~CANBusController();
    static CANBusController& GetInstance();

    void ResetCache();
    void Set(int id, float val);
    float Get(int id);

    void SetPID(int id, double p, double i, double d);
    void SetPositionReference(int id, CANJaguar::PositionReference reference);
    void SetPotentiometerTurns(int id, UINT16 turns);
    CANJaguar::PositionReference BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetPositionReference);

    void SetControlMode(int id, CANJaguar::ControlMode controlMode);
    CANJaguar::ControlMode GetControlMode(int id);
    void EnableControl(int id, double encoderInitialPosition = 0.0);
    void BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(DisableControl);

    float BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetOutputCurrent);
    float BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetTemperature);
    float BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetBusVoltage);
    float BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetOutputVoltage);

    double BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetSpeed);
    double BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetPosition);

    void ConfigNeutralMode(int id, CANJaguar::NeutralMode mode);
    void PrintOnlineStatus();

    void ConfigSoftPositionLimits(int id, double forwardLimitPosition, double reverseLimitPosition);
    void BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(DisableSoftPositionLimits);
    bool BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetForwardLimitOK);
    bool BLOCKING_WRAP_CANJAGUAR_FUNCTION_NOPARAM(GetReverseLimitOK);

private:
    CANBusController();
    DISALLOW_COPY_AND_ASSIGN(CANBusController);

    static void BusWriterTaskRunner();
    void BusWriterTask();
    int BusIdToIndex(int id);

    const static int kMinJaguarId = 20;
    const static int kMaxJaguarId = 28;
    const static int kNumJaguars = kMaxJaguarId - kMinJaguarId + 1;

    volatile float setpoints[kNumJaguars];
    volatile bool setpointChanged[kNumJaguars];

    volatile double gains[kNumJaguars][3];
    volatile bool gainsChanged[kNumJaguars];

    volatile CANJaguar::PositionReference positionReferences[kNumJaguars];
    volatile bool positionReferencesChanged[kNumJaguars];

    volatile CANJaguar::NeutralMode neutralModes[kNumJaguars];
    volatile bool neutralModeChanged[kNumJaguars];

    SafeCANJaguar* jaguars[kNumJaguars];
    static CANBusController* instance;

    Task busWriterTask;
    SEM_ID semaphore;

    volatile int forceSetpointUpdate;
};

#endif
