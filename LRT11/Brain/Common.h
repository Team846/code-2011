#include "Brain.h"

void Brain::Common()
{
    inputs.UpdateDebouncing();
    config.UpdateAssignableDials();

    console.UpdateCycleCount();
    lcd.UpdateHeartbeat(inputs.IsServiceMode());

    action.Config.load = inputs.ShouldLoadConfig();
    action.Config.save = inputs.ShouldSaveConfig();
    action.Config.apply = inputs.ShouldApplyConfig();

}
