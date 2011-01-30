#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "..\General.h"
#include "..\ActionData.h"

class Component
{
protected:
    ActionData& action;

public:
    Component()
        :  action(ActionData::GetInstance()) { }

    virtual void Output() = 0;
};

#endif
