#ifndef LRT_CANBUSCTRLR_H_
#define LRT_CANBUSCTRLR_H_

#include "..\General.h"
#include "..\Jaguar\SafeCANJaguar.h"
#include "..\Util\AsynchronousPrinter.h"

class CANBusController : public SensorBase
{
public:
    virtual ~CANBusController();
    static CANBusController& GetInstance();

    void Set(int id, float val);
    float Get(int id);

    float GetCurrent(int id);
    float GetTemperature(int id);
    float GetBatteryVoltage(int id);
    float GetOutputVoltage(int id);

    void ConfigNeutralMode(int id, CANJaguar::NeutralMode mode);
    void PrintOnlineStatus();

private:
    CANBusController();
    DISALLOW_COPY_AND_ASSIGN(CANBusController)
    ;

    static void BusWriterTaskRunner();
    void BusWriterTask();
    int BusIdToIndex(int id);

    const static int kMinJaguarId = 2;
    const static int kMaxJaguarId = 5;
    const static int kNumJaguars = kMaxJaguarId - kMinJaguarId + 1;

    volatile float setpoints[kNumJaguars];
    volatile bool setpointChanged[kNumJaguars];

    volatile CANJaguar::NeutralMode neutralModes[kNumJaguars];
    volatile bool neutralModeChanged[kNumJaguars];

    SafeCANJaguar* jaguars[kNumJaguars];
    static CANBusController* instance;

    Task busWriterTask;
    SEM_ID semaphore;
};

#endif
