#include "Brain.h"

void Brain::Common()
{
    inputs.UpdateDebouncing();
    config.UpdateAssignableDials();

    console.UpdateCycleCount();
    lcd.UpdateHeartbeat(inputs.IsServiceMode());
}
