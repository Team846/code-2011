#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "..\General.h"
#include "..\ActionData.h"
#include <string>
#include <list>
using namespace std;


class Component
{
protected:
    ActionData& action;
public:
    Component()
        :  action(ActionData::GetInstance()) { }

    virtual void Output() = 0;
    virtual string GetName() = 0;

};

typedef struct ComponentData
{
    bool RequiresEnabledState;
    const static int NO_DISABLE_DIO = -1;
    int DIO;
};

typedef pair < Component*, ComponentData> ComponentWithData;
namespace ComponentFactory
{
    list < ComponentWithData >* CreateComponents();
}

#endif
