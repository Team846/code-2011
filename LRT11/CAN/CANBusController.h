#ifndef CAN_BUS_CONTROLLER_H_
#define CAN_BUS_CONTROLLER_H_

#include "..\General.h"
#include "..\Jaguar\SafeCANJaguar.h"
#include "..\Util\AsynchronousPrinter.h"

/*
 * This retrieval macro is used to return SafeCANJaguar data (temperature,
 * current, etc.) in a blocking manner. This only works with no parameter
 * methods and has been added to reduce code size and prevent errors (60+
 * lines of repetition without it).
 */
#define RETRIEVE_JAGUAR_DATA_BLOCKING(method) method(int id) \
    {                                   \
        int idx = BusIdToIndex(id);     \
        return jaguars[idx]->method();       \
    }

class CANBusController : public SensorBase
{
public:
    virtual ~CANBusController();
    static CANBusController& GetInstance();

    void ResetCache();
    void ResetCache(int id);
    void Set(int id, float val);
    float Get(int id);

    // blocking jaguar configuration functions
    void SetPID(int id, double p, double i, double d);
    void SetPositionReference(int id, CANJaguar::PositionReference reference);
    void SetPotentiometerTurns(int id, UINT16 turns);
    CANJaguar::PositionReference RETRIEVE_JAGUAR_DATA_BLOCKING(GetPositionReference);

    void SetControlMode(int id, CANJaguar::ControlMode controlMode);
    CANJaguar::ControlMode GetControlMode(int id);
    void EnableControl(int id, double encoderInitialPosition = 0.0);

    // blocking functions with no parameter
    void RETRIEVE_JAGUAR_DATA_BLOCKING(DisableControl);

    float RETRIEVE_JAGUAR_DATA_BLOCKING(GetOutputCurrent);
    float RETRIEVE_JAGUAR_DATA_BLOCKING(GetTemperature);
    float RETRIEVE_JAGUAR_DATA_BLOCKING(GetBusVoltage);
    float RETRIEVE_JAGUAR_DATA_BLOCKING(GetOutputVoltage);

    double RETRIEVE_JAGUAR_DATA_BLOCKING(GetSpeed);
    double RETRIEVE_JAGUAR_DATA_BLOCKING(GetPosition);

    void ConfigNeutralMode(int id, CANJaguar::NeutralMode mode);
    void PrintOnlineStatus();

    void ConfigSoftPositionLimits(int id, double forwardLimitPosition, double reverseLimitPosition);
    void RETRIEVE_JAGUAR_DATA_BLOCKING(DisableSoftPositionLimits);
    bool RETRIEVE_JAGUAR_DATA_BLOCKING(GetForwardLimitOK);
    bool RETRIEVE_JAGUAR_DATA_BLOCKING(GetReverseLimitOK);

private:
    CANBusController();
    DISALLOW_COPY_AND_ASSIGN(CANBusController);

    static void BusWriterTaskRunner();
    void BusWriterTask();
    int BusIdToIndex(int id);
    
    // CAN jaguar ids (should be a contiguous block)
#ifdef LRT_ROBOT_2011
    const static int kMinJaguarId = 20;
    const static int kMaxJaguarId = 28;
#else
    const static int kMinJaguarId = 2;
    const static int kMaxJaguarId = 5;
#endif
    
    const static int kNumJaguars = kMaxJaguarId - kMinJaguarId + 1;

    // arrays for storing values and caching
    volatile float setpoints[kNumJaguars];
    volatile bool setpointChanged[kNumJaguars];

    // no caching for gains and position reference anymore because they
    // are only called during configuration
//    volatile double gains[kNumJaguars][3];
//    volatile bool gainsChanged[kNumJaguars];

//    volatile CANJaguar::PositionReference positionReferences[kNumJaguars];
//    volatile bool positionReferencesChanged[kNumJaguars];

    volatile CANJaguar::NeutralMode neutralModes[kNumJaguars];
    volatile bool neutralModeChanged[kNumJaguars];

    SafeCANJaguar* jaguars[kNumJaguars];
    static CANBusController* instance;

    Task busWriterTask;
    SEM_ID semaphore;

};

#endif
