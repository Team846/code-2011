#include "VirtualCANBusController.h"

VirtualCANBusController* VirtualCANBusController::instance = NULL;

VirtualCANBusController& VirtualCANBusController::GetInstance()
{
    if(instance == NULL)
        instance = new VirtualCANBusController();
    return *instance;
}

VirtualCANBusController::VirtualCANBusController()
{
    AddToSingletonList();
}

void VirtualCANBusController::Set(int id, float val)
{
    char buffer[20];
    sprintf(buffer, "Esc %d Setpoint: ", id);
    SmartDashboard::Log(val, buffer);
}
