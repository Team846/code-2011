#include "Brain.h"

void Brain::TeleopMinibot()
{
    if(inputs.ShouldDeployLegs())
        action.deployer.shouldAlignerRelease = true;

    if(isFinale && inputs.ShouldDeployMinibot())
        action.deployer.shouldDeployMinibot = true;
    else
        action.deployer.shouldDeployMinibot = false;
}
