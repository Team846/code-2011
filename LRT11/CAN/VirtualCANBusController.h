#ifndef VIRTUAL_CAN_BUS_CONTROLLER_H_
#define VIRTUAL_CAN_BUS_CONTROLLER_H_

#include "..\General.h"
#include "..\Jaguar\SafeCANJaguar.h"
#include "..\Util\AsynchronousPrinter.h"

class VirtualCANBusController : public SensorBase
{
public:
    virtual ~VirtualCANBusController() {}
    static VirtualCANBusController& GetInstance();

    void ResetCache() {}
    void ResetCache(int id) {}
    void Set(int id, float val);

    float Get(int id)
    {
        return 0.0;
    }

    // blocking jaguar configuration functions
    void SetPID(int id, double p, double i, double d) {}
    void SetPositionReference(int id, CANJaguar::PositionReference reference) {}
    void SetPotentiometerTurns(int id, UINT16 turns) {}

    CANJaguar::PositionReference GetPositionReference(int channel)
    {
        return CANJaguar::kPosRef_None;
    }

    void SetControlMode(int id, CANJaguar::ControlMode controlMode) {}

    CANJaguar::ControlMode GetControlMode(int id)
    {
        return CANJaguar::kPercentVbus;
    }

    void EnableControl(int id, double encoderInitialPosition = 0.0) {}

    // blocking functions with no parameter
    void DisableControl(int channel) {}

    float GetOutputCurrent(int channel)
    {
        return 0.0;
    }

    float GetTemperature(int channel)
    {
        return 0.0;
    }

    float GetBusVoltage(int channel)
    {
        return 0.0;
    }

    float GetOutputVoltage(int channel)
    {
        return 0.0;
    }

    double GetSpeed(int channel)
    {
        return 0.0;
    }

    double GetPosition(int channel)
    {
        return 0.0;
    }

    void ConfigNeutralMode(int id, CANJaguar::NeutralMode mode) {}
    void PrintOnlineStatus() {}

private:
    VirtualCANBusController();
    DISALLOW_COPY_AND_ASSIGN(VirtualCANBusController);

    static VirtualCANBusController* instance;
};

#endif
