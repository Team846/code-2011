#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "..\General.h"
#include "..\ActionData.h"
#include <string>
using namespace std;

class Component
{
protected:
    ActionData& action;

public:
    Component()
        :  action(ActionData::GetInstance()) { }

    virtual void Output() = 0;
    virtual string GetName(); //TODO make this a pure virtual function
};

#endif
