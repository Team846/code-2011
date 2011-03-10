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
    {
        Synchronized s(semaphore);
        setpoints[id] = val;
    }

    char buffer[20];
    sprintf(buffer, "Esc %d Setpoint: ", id);
    SmartDashboard::Log(val, buffer);
}

void VirtualCANBusController::SetDutyCycleSubscriber(int channel, DutyCycleSubscriber* subscriber)
{
    Synchronized s(semaphore);
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
