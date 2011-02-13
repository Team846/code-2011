#include "Configurable.h"
#include "Config.h"

Configurable::Configurable()
{
    Config::RegisterConfigurable(this);
    this->Configure(); // must keep this-> because Configure is pure virtual
}
