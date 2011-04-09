#include "Brain.h"

void Brain::Common()
{
    action.wasDisabled = wasDisabled;

    inputs.UpdateDebouncing();
    config.UpdateAssignableDials();

    console.UpdateCycleCount();
    lcd.UpdateHeartbeat(inputs.IsServiceMode());

    // load, save, and apply buttons should work in all modes
    action.config.load = inputs.ShouldLoadConfig();
    action.config.save = inputs.ShouldSaveConfig();
    action.config.apply = inputs.ShouldApplyConfig();
}
