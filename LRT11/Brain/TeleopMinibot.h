#include "Brain.h"

Brain::TeleopMinibot()
{
    if(inputs.ShouldDeployLegs())
        action.deployer.alignerReleased = true;

    if(inputs.ShouldDeployMinibot())
        action.deployer.state = action.deployer.deployed;
    else
        action.deployer.state = action.deployer.retracted;

}