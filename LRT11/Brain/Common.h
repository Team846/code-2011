#include "Brain.h"

void Brain::Common()
{
    inputs.UpdateDebouncing();
    config.UpdateAssignableDials();

    console.UpdateCycleCount();
    lcd.UpdateHeartbeat(inputs.IsServiceMode());

    action.config.load = inputs.ShouldLoadConfig();
    action.config.save = inputs.ShouldSaveConfig();
    action.config.apply = inputs.ShouldApplyConfig();
}
