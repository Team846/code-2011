#include "ActionData.h"

ActionData* ActionData::instance = NULL;

ActionData& ActionData::GetInstance()
{
    if(instance == NULL)
        instance = new ActionData();
    return *instance;
}

ActionData::ActionData()
{
    AddToSingletonList();
}

ActionData::~ActionData()
{

}
