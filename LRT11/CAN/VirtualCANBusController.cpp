#include "VirtualCANBusController.h"

VirtualCANBusController* VirtualCANBusController::instance = NULL;

VirtualCANBusController& VirtualCANBusController::GetInstance()
{
    if(instance == NULL)
        instance = new VirtualCANBusController();
    return *instance;
}

VirtualCANBusController::VirtualCANBusController()
    : busWriterTask("VirtualCANBusController",
            (FUNCPTR)VirtualCANBusController::BusWriterTaskRunner)
    , semaphore(semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE
            | SEM_INVERSION_SAFE))
{
    AddToSingletonList();

    // fill arrays with default values
    for(int id = kMinJaguarId; id <= kMaxJaguarId; ++id)
    {
        int idx = BusIdToIndex(id);
        setpoints[idx] = 0.0;
        subscribers[idx] = NULL;
        // idx is the array index; id is the CAN-bus ID
        sprintf(jaguarLabels[idx], "Esc %d Setpoint: ", id);
    }

    busWriterTask.Start();
}

int VirtualCANBusController::BusIdToIndex(int id)
{
    if(id > kMaxJaguarId || id < kMinJaguarId)
    {
        AsynchronousPrinter::Printf("VirtualCANBusController: %d out of range!\n", id);
        return 0;
    }

    return id - kMinJaguarId;
}

void VirtualCANBusController::Set(int id, float val)
{
    int idx = BusIdToIndex(id);
    setpoints[idx] = val;
    SmartDashboard::Log(val, jaguarLabels[idx]);
}

void VirtualCANBusController::SetDutyCycleSubscriber(int channel, DutyCycleSubscriber* subscriber)
{
    int idx = BusIdToIndex(channel);
    subscribers[idx] = subscriber;
}

void VirtualCANBusController::BusWriterTaskRunner()
{
    VirtualCANBusController::GetInstance().BusWriterTask();
}

void VirtualCANBusController::BusWriterTask()
{
    while(true)
    {
        for(int id = kMinJaguarId; id <= kMaxJaguarId; ++id)
        {
            int idx = BusIdToIndex(id);

            if(subscribers[idx] != NULL)
                subscribers[idx]->Update(setpoints[idx]);
        }

        Wait(0.02); // run at 50 Hz
    }
}
