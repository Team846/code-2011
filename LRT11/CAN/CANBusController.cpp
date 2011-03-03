#include "CANBusController.h"

CANBusController* CANBusController::instance = NULL;

CANBusController& CANBusController::GetInstance()
{
    if(instance == NULL)
        instance = new CANBusController();
    return *instance;
}

CANBusController::CANBusController()
    : busWriterTask("CANBusController",
            (FUNCPTR)CANBusController::BusWriterTaskRunner)
    , semaphore(semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE
            | SEM_INVERSION_SAFE))
{
    AddToSingletonList();

    // fill arrays with default values
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

void CANBusController::ResetCache()
{
    Synchronized s(semaphore);

    for(int id = kMinJaguarId; id <= kMaxJaguarId; ++id)
        ResetCache(id);
}

void CANBusController::ResetCache(int id)
{
    Synchronized s(semaphore);

    int idx = BusIdToIndex(id);
    setpoints[idx] = -91387; // bogus value that will ensure a setpoint change
    setpointChanged[idx] = false;
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

    // caching; only set if changed
    if(setpoints[idx] != val)
    {
        setpoints[idx] = val;
        setpointChanged[idx] = true;
    }
}

// blocking function, as it is only called during jaguar configuration
void CANBusController::SetPID(int id, double p, double i, double d)
{
    int idx = BusIdToIndex(id);
    jaguars[idx]->SetPID(p, i, d);
}

float CANBusController::Get(int id)
{
    Synchronized s(semaphore);

    // get from the cache
    int idx = BusIdToIndex(id);
    return setpoints[idx];
}

void CANBusController::ConfigNeutralMode(int id, CANJaguar::NeutralMode mode)
{
    Synchronized s(semaphore);
    int idx = BusIdToIndex(id);

    // caching; only set if changed
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
                // get the data to update
                Synchronized s(semaphore);

                setpoint = setpoints[idx];
                setpointChanged = this->setpointChanged[idx];

                mode = neutralModes[idx];
                neutralModeChanged = this->neutralModeChanged[idx];
            }

            // set new values only if the old ones changed
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

/*
 * All the following are blocking functions, as they are
 * only called during jaguar configuration.
 */

void CANBusController::SetPositionReference(int id, CANJaguar::PositionReference reference)
{
    int idx = BusIdToIndex(id);
    jaguars[idx]->SetPositionReference(reference);
}

void CANBusController::SetPotentiometerTurns(int id, UINT16 turns)
{
    int idx = BusIdToIndex(id);
    jaguars[idx]->ConfigPotentiometerTurns(turns);
}

void CANBusController::SetControlMode(int id, CANJaguar::ControlMode controlMode)
{
    int idx = BusIdToIndex(id);
    jaguars[idx]->ChangeControlMode(controlMode);
}

CANJaguar::ControlMode CANBusController::GetControlMode(int id)
{
    int idx = BusIdToIndex(id);
    return jaguars[idx]->GetControlMode();
}

void CANBusController::EnableControl(int id, double encoderInitialPosition)
{
    int idx = BusIdToIndex(id);
    return jaguars[idx]->EnableControl(encoderInitialPosition);
}

void CANBusController::ConfigSoftPositionLimits(int id, double forwardLimitPosition, double reverseLimitPosition)
{
    int idx = BusIdToIndex(id);
    jaguars[idx]->ConfigSoftPositionLimits(forwardLimitPosition, reverseLimitPosition);
}
