#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "..\General.h"
#include "..\ActionData.h"
#include <string>
#include <list>
using namespace std;

class Component;

typedef struct ComponentData
{
    bool RequiresEnabledState;
    const static int NO_DS_DISABLE_DIO = -1;
    int DS_DIOToDisableComponent;
};
typedef pair < Component*, ComponentData> ComponentWithData;

class Component
{
protected:
    ActionData& action;
public:
    Component()
        :  action(ActionData::GetInstance()) { }

    virtual void Output() = 0;
    virtual string GetName() = 0;

    static list < ComponentWithData >* CreateComponents();

private:
    static ComponentData createComponentData(bool RequiresEnabledState, int DIO);

};


#endif
