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
    , forceSetpointUpdate(-1)
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

void CANBusController::ResetCache()
{
    Synchronized s(semaphore);

    // force update all setpoints in 15 cycles after
    // a switch in game state; must have delay for
    // successful speed controller update
    forceSetpointUpdate = 15;
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

void CANBusController::SetPID(int id, double p, double i, double d)
{
    int idx = BusIdToIndex(id);
    jaguars[idx]->SetPID(p, i, d);
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
            bool forceSetpointUpdate;

            CANJaguar::NeutralMode mode;
            bool setpointChanged, neutralModeChanged;

            {
                Synchronized s(semaphore);

                setpoint = setpoints[idx];
                setpointChanged = this->setpointChanged[idx];

                mode = neutralModes[idx];
                neutralModeChanged = this->neutralModeChanged[idx];

                forceSetpointUpdate = this->forceSetpointUpdate;
            }

            if(setpointChanged || forceSetpointUpdate == 0)
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

        {
            Synchronized s(semaphore);
            if(forceSetpointUpdate >= 0)
                forceSetpointUpdate--;
        }

        Wait(0.001);   // allow other tasks to run
    }
}

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
