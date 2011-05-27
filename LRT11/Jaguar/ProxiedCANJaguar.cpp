#include <stdio.h>
#include "ProxiedCANJaguar.h"
#include "../CAN/JaguarReader.h"
#define DISABLE_SETPOINT_CACHING 0

GameState ProxiedCANJaguar::gameState = DISABLED;
ProxiedCANJaguar::JaguarList ProxiedCANJaguar::jaguars = {0};

ProxiedCANJaguar::ProxiedCANJaguar(UINT8 channel, char* name)
    : LRTCANJaguar(channel)
    , taskName_("JAG#" + Util::ToString<int>(channel))
    , print_ctor_dtor(taskName_.c_str(), (taskName_ + "\n").c_str())
    , channel(channel)
    , name_(name)
    , setpoint(0.0)
    , lastSetpoint(0.0)
    , shouldCacheSetpoint(false)
    , cacheSetpointCounter(0)
    , mode(LRTCANJaguar::kNeutralMode_Coast)
    , lastMode(LRTCANJaguar::kNeutralMode_Coast)
    , shouldCacheMode(false)
    , current(0.0)
    , collectCurrent(false)
    , potValue(0.0)
    , collectPotValue(false)
#ifdef VIRTUAL
    , controller(VirtualCANBusController::GetInstance())
#else
    , lastState(DISABLED)
//    : controller(CANBusController::GetInstance())
#endif
    , index(jaguars.num)
    , commTask(taskName_.c_str(), (FUNCPTR) ProxiedCANJaguar::CommTaskWrapper)
    , commSemaphore(semBCreate(SEM_Q_PRIORITY, SEM_EMPTY))
    , running_(false)
    , quitting_(false)
{
    jaguars.j[jaguars.num++] = this;

    jaguars.currents[index] = 0;
    jaguars.shouldCollectCurrent[index] = false;

    jaguars.potValues[index] = 0;
    jaguars.shouldCollectPotValue[index] = false;

    if(name_ == NULL) name_ = "?";
    commTask.Start((UINT32) this);
    printf("Created Jaguar %2d: %s\n", channel, name_);
}

ProxiedCANJaguar::~ProxiedCANJaguar()
{
    JaguarReader::GetInstance().StopTask(); //is this the best place? -dg TODO
    StopBackgroundTask();

    //leave the semaphore; we are quitting anyway. -dg. Is this Bad?
}
void ProxiedCANJaguar::StopBackgroundTask()
{
    if(running_)
    {
        INT32 task_id = commTask.GetID();
        commTask.Stop();
        printf("Task 0x%x killed for CANid=%d:%s\n", task_id, channel, name_);
    }
}

void ProxiedCANJaguar::ShouldCollectCurrent(bool shouldCollect)
{
//    jaguars.shouldCollectCurrent[index] = true;
    collectCurrent = shouldCollect;
}

void ProxiedCANJaguar::ShouldCollectPotValue(bool shouldCollect)
{
//    jaguars.shouldCollectPotValue[index] = shouldCollect;
    collectPotValue = shouldCollect;
}

float ProxiedCANJaguar::GetCurrent()
{
//    if(!jaguars.shouldCollectCurrent[index])
//    {
//        AsynchronousPrinter::Printf("Fatal %s:%d\n", __FILE__, __LINE__);
//        return -1.0;
//    }

//    return jaguars.currents[index];
    return current;
}

float ProxiedCANJaguar::GetPotValue()
{
//    if(!jaguars.shouldCollectPotValue[index])
//    {
//        AsynchronousPrinter::Printf("Fatal %s:%d\n", __FILE__, __LINE__);
//        return -1.0;
//    }

//    return jaguars.potValues[index];
    return potValue;
}

int ProxiedCANJaguar::CommTaskWrapper(UINT32 proxiedCANJaguarPointer)
{
    ProxiedCANJaguar* jaguar = (ProxiedCANJaguar*) proxiedCANJaguarPointer;
    if(jaguar->channel != 0)    // ignore jags on channel #0 -dg
    {
        jaguar->running_ = true;
        jaguar->CommTask();
    }
    jaguar->running_ = false;
    printf("Ending task for Jaguar %s %d\n", jaguar->name_, jaguar->channel);
    return 0; // return no error
}

void ProxiedCANJaguar::CommTask()
{
    while(!quitting_)
    {
        semTake(commSemaphore, WAIT_FOREVER);
        if(quitting_)
            break;

        if(setpoint == lastSetpoint && lastState == gameState)
            shouldCacheSetpoint = true;
        else
            shouldCacheSetpoint = false;

#if DISABLE_SETPOINT_CACHING
        shouldCacheSetpoint = false;
#endif

        // cache if setpoint has changed or if value has been cached for over
        // half a second -KV -DG championships 4/28/11
        if(!shouldCacheSetpoint || cacheSetpointCounter > 25)
        {
            LRTCANJaguar::Set(setpoint);
            lastSetpoint = setpoint;
            cacheSetpointCounter = 0;
        }
        else
            cacheSetpointCounter++;

        if(mode == lastMode && lastState == gameState)
            shouldCacheMode = true;
        else
            shouldCacheMode = false;

        if(!shouldCacheMode)
        {
            LRTCANJaguar::ConfigNeutralMode(mode);
            lastMode = mode;
        }

        if(collectCurrent)
        {
            float current = LRTCANJaguar::GetOutputCurrent();
            if(StatusOK())
                this->current = current;
            else
                AsynchronousPrinter::Printf("Invalid current; not storing\n");
        }

        if(collectPotValue)
        {
            float potValue = LRTCANJaguar::GetPosition();
            if(StatusOK())
                this->potValue = potValue;
            else
                AsynchronousPrinter::Printf("Invalid pot value; not storing\n");
        }

        lastState = gameState;
//        AsynchronousPrinter::Printf("%d\n", channel);
    }
}

void ProxiedCANJaguar::BeginComm()
{
    semGive(commSemaphore);
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
//    if(setpoint != lastSetpoint || lastState != gameState)
//        LRTCANJaguar::Set(setpoint);

    this->setpoint = setpoint;

//    lastSetpoint = setpoint;
//    lastState = gameState;
}

void ProxiedCANJaguar::ConfigNeutralMode(NeutralMode mode)
{
    this->mode = mode;
}

void ProxiedCANJaguar::SetGameState(GameState state)
{
    gameState = state;
}

void ProxiedCANJaguar::ResetCache()
{
    // bogus value to reset cache
    lastSetpoint = -1.0e6;
}
#endif
