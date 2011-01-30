#ifndef ACTION_DATA_H_
#define ACTION_DATA_H_

#include "General.h"

class ActionData : public SensorBase
{
private:
    static ActionData* instance;
    
public:
    ~ActionData();
    static ActionData& GetInstance();
    
    // drive train
    struct {
    	float rawForward, rawTurn;
    	bool brakeLeft, brakeRight;
    } driveTrain;

protected:
    ActionData();
    DISALLOW_COPY_AND_ASSIGN(ActionData);
};

#endif
