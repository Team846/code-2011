#include "Brain.h"

void Brain::Common()
{
    inputs.UpdateDebouncing();
    config.UpdateAssignableDials();

    console.UpdateCycleCount();
    lcd.UpdateHeartbeatAndScrolling(inputs.ShouldLCDScrollUp(),
            inputs.ShouldLCDScrollDown(), inputs.GetServiceMode());
}
