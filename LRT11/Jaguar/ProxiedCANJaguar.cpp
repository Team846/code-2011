#include "ProxiedCANJaguar.h"

GameState ProxiedCANJaguar::gameState = DISABLED;
ProxiedCANJaguar::JaguarList ProxiedCANJaguar::jaguars = {0};

ProxiedCANJaguar::ProxiedCANJaguar(UINT8 channel)
    : CANJaguar(channel)
    , StartOfCycleSubscriber()
#ifdef VIRTUAL
    , controller(VirtualCANBusController::GetInstance())
    , channel(channel)
#else
    , lastState(DISABLED)
//    : controller(CANBusController::GetInstance())
#endif
    , index(jaguars.num)
#ifdef NON_BLOCKING
    , writerTask((Util::ToString<int>(channel) + "jagSetTask").c_str(), (FUNCPTR)ProxiedCANJaguar::setThreadEntryPoint)
#endif
{
    jaguars.j[jaguars.num++] = this;

    jaguars.currents[index] = 0;
    jaguars.shouldCollectCurrent[index] = false;

    jaguars.potValues[index] = 0;
    jaguars.shouldCollectPotValue[index] = false;

#ifdef NON_BLOCKING
    setSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
    writerTask.Start((UINT32) this);
#endif
}

ProxiedCANJaguar::~ProxiedCANJaguar()
{

}

void ProxiedCANJaguar::CollectCurrent()
{
    jaguars.shouldCollectCurrent[index] = true;
}

void ProxiedCANJaguar::CollectPotValue()
{
    jaguars.shouldCollectPotValue[index] = true;
}

float ProxiedCANJaguar::GetCurrent()
{
    if(!jaguars.shouldCollectCurrent[index])
    {
        AsynchronousPrinter::Printf("Fatal %s:%s\n", __FILE__, __LINE__);
        return -1.0;
    }

    return jaguars.currents[index];
}

#ifdef NON_BLOCKING
int ProxiedCANJaguar::setThreadEntryPoint(UINT32 proxiedCANJaguarPointer)
{
    ProxiedCANJaguar* jaguar = (ProxiedCANJaguar*) proxiedCANJaguarPointer;
    return jaguar->setThread();
}

int ProxiedCANJaguar::setThread()
{
    while(true)
    {
        semTake(setSemaphore, WAIT_FOREVER);
        if(shouldSetSetPoint)
            CANJaguar::Set(setPoint);
        if(shouldSetNeutralMode)
            CANJaguar::ConfigNeutralMode(neutralMode);
    }
}
#endif

void ProxiedCANJaguar::NewCycle()
{
#ifdef NON_BLOCKING
    shouldSetSetPoint    = false;
    shouldSetNeutralMode = false;
#endif
}

#ifdef VIRTUAL
void ProxiedCANJaguar::Set(float setpoint, UINT8 syncGroup)
{
    controller.Set(channel, setpoint);
}

void ProxiedCANJaguar::SetPID(double p, double i, double d)
{
    controller.SetPID(channel, p, i, d);
}

float ProxiedCANJaguar::Get()
{
    return controller.Get(channel);
}

void ProxiedCANJaguar::Disable()
{
    controller.Set(channel, 0);
}

float ProxiedCANJaguar::GetCurrent()
{
    return controller.GetOutputCurrent(channel);
}

float ProxiedCANJaguar::GetTemperature()
{
    return controller.GetTemperature(channel);
}

float ProxiedCANJaguar::GetBatteryVoltage()
{
    return controller.GetBusVoltage(channel);
}

float ProxiedCANJaguar::GetOutputVoltage()
{
    return controller.GetOutputVoltage(channel);
}

double ProxiedCANJaguar::GetPosition()
{
    return controller.GetPosition(channel);
}

double ProxiedCANJaguar::GetSpeed()
{
    return controller.GetSpeed(channel);
}

void ProxiedCANJaguar::ConfigNeutralMode(CANJaguar::NeutralMode mode)
{
    controller.ConfigNeutralMode(channel, mode);
}

void ProxiedCANJaguar::SetPositionReference(VirtualPot* reference)
{
    controller.SetPositionReference(channel, reference);
}

void ProxiedCANJaguar::ConfigPotentiometerTurns(UINT16 turns)
{
    controller.SetPotentiometerTurns(channel, turns);
}

CANJaguar::PositionReference ProxiedCANJaguar::GetPositionReference()
{
    return controller.GetPositionReference(channel);
}

void ProxiedCANJaguar::ChangeControlMode(CANJaguar::ControlMode controlMode)
{
    controller.SetControlMode(channel, controlMode);
}

CANJaguar::ControlMode ProxiedCANJaguar::GetControlMode()
{
    return controller.GetControlMode(channel);
}

void ProxiedCANJaguar::EnableControl(double encoderInitialPosition)
{
    controller.EnableControl(channel, encoderInitialPosition);
}

void ProxiedCANJaguar::DisableControl()
{
    controller.DisableControl(channel);
}

void ProxiedCANJaguar::ResetCache()
{
    controller.ResetCache(channel);
}
#else

void ProxiedCANJaguar::Set(float setpoint, UINT8 syncGroup)
{
    // send the value if there is a setpoint or game state change
    if(setpoint != lastSetpoint || lastState != gameState)
    {
#ifdef NON_BLOCKING
        setPoint = setpoint;
        shouldSetSetPoint = true;
#else
        CANJaguar::Set(setpoint);
#endif
    }
#ifdef NON_BLOCKING
    else
        shouldSetSetPoint = false;
    semGive(setSemaphore);
#endif

    lastSetpoint = setpoint;
    lastState = gameState;
}

void ProxiedCANJaguar::ConfigNeutralMode(NeutralMode neutralMode)
{
#ifdef NON_BLOCKING
    if(neutralMode == lastNeutralMode)
        return;
    shouldSetNeutralMode = true;
    this->neutralMode = neutralMode;
#else
    CANJaguar::ConfigNeutralMode(neutralMode);
#endif
}
void ProxiedCANJaguar::SetGameState(GameState state)
{
    gameState = state;
}

void ProxiedCANJaguar::ResetCache()
{
#ifdef NON_BLOCKING
    shouldSetSetPoint = true;
    shouldSetNeutralMode = true;
    semGive(setSemaphore);
#else
    // bogus value to reset cache
    lastSetpoint = -1.0e6;
#endif
}
#endif
