#include "Component.h"

class Config;

class ConfigLoader : public Component
{
public:
    ConfigLoader();
    virtual ~ConfigLoader();

    virtual void Output();
    virtual string GetName();
private:
    Config& config;
};
