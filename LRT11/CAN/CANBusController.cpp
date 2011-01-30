#include "CANBusController.h"

CANBusController* CANBusController::instance = NULL;

CANBusController& CANBusController::GetInstance()
{
    if(instance == NULL)
        instance = new CANBusController();
    return *instance;
}

CANBusController::CANBusController() :
    busWriterTask("CANBusController",
            (FUNCPTR)CANBusController::BusWriterTaskRunner),
    semaphore(semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE
            | SEM_INVERSION_SAFE))
{
    AddToSingletonList();

    for(int id = kMinJaguarId; id <= kMaxJaguarId; ++id)
    {
        int idx = BusIdToIndex(id);
        jaguars[idx] = new SafeCANJaguar(id);
        jaguars[idx]->SetSafetyEnabled(false);
        setpoints[idx] = 0.0;
        neutralModes[idx] = CANJaguar::kNeutralMode_Jumper;
        setpointChanged[idx] = neutralModeChanged[idx] = false;
    }

    busWriterTask.Start();
}

CANBusController::~CANBusController()
{
    for(int id = kMinJaguarId; id <= kMaxJaguarId; ++id)
    {
        int idx = BusIdToIndex(id);
        delete jaguars[idx];
    }

    semDelete(semaphore);
    semaphore = NULL;
}

void CANBusController::Set(int id, float val)
{
    Synchronized s(semaphore);
    int idx = BusIdToIndex(id);

    if(setpoints[idx] != val)
    {
        setpoints[idx] = val;
        setpointChanged[idx] = true;
    }
}
float CANBusController::Get(int id)
{
    Synchronized s(semaphore);

    int idx = BusIdToIndex(id);
    return setpoints[idx];
}

void CANBusController::ConfigNeutralMode(int id, CANJaguar::NeutralMode mode)
{
    Synchronized s(semaphore);
    int idx = BusIdToIndex(id);

    if(neutralModes[idx] != mode)
    {
        neutralModes[idx] = mode;
        neutralModeChanged[idx] = true;
    }
}

void CANBusController::PrintOnlineStatus()
{
    for(int id = kMinJaguarId; id <= kMaxJaguarId; ++id)
    {
        int idx = BusIdToIndex(id);
        jaguars[idx]->IsOnline();
    }
}

int CANBusController::BusIdToIndex(int id)
{
    if(id > kMaxJaguarId || id < kMinJaguarId)
    {
        AsynchronousPrinter::Printf("CANBusController: %d out of range!\n", id);
        return 0;
    }

    return id - kMinJaguarId;
}

void CANBusController::BusWriterTaskRunner()
{
    CANBusController::GetInstance().BusWriterTask();
}

void CANBusController::BusWriterTask()
{
    while(true)
    {
        for(int id = kMinJaguarId; id <= kMaxJaguarId; ++id)
        {
            int idx = BusIdToIndex(id);
            float setpoint;

            CANJaguar::NeutralMode mode;
            bool setpointChanged, neutralModeChanged;

            {
                Synchronized s(semaphore);

                setpoint = setpoints[idx];
                setpointChanged = this->setpointChanged[idx];

                mode = neutralModes[idx];
                neutralModeChanged = this->neutralModeChanged[idx];
            }

            if(setpointChanged)
            {
                jaguars[idx]->Set(setpoint);

                {
                    Synchronized s(semaphore);
                    this->setpointChanged[idx] = false;
                }
            }

            if(neutralModeChanged)
            {
                jaguars[idx]->ConfigNeutralMode(mode);

                {
                    Synchronized s(semaphore);
                    this->neutralModeChanged[idx] = false;
                }
            }
        }

        Wait(0.001);   // allow other tasks to run
    }
}

float CANBusController::GetCurrent(int id)
{
    int idx = BusIdToIndex(id);
    return jaguars[idx]->GetOutputCurrent();
}

float CANBusController::GetTemperature(int id)
{
    int idx = BusIdToIndex(id);
    return jaguars[idx]->GetTemperature();
}

float CANBusController::GetBatteryVoltage(int id)
{
    int idx = BusIdToIndex(id);
    return jaguars[idx]->GetBusVoltage();
}

float CANBusController::GetOutputVoltage(int id)
{
    int idx = BusIdToIndex(id);
    return jaguars[idx]->GetOutputVoltage();
}
