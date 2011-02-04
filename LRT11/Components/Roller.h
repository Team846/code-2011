#ifndef ROLLER_H_
#define ROLLER_H_

#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "..\Util\Util.h"

class Roller : public Component
{
private:
    ProxiedCANJaguar topRoller;
    ProxiedCANJaguar bottomRoller;

public:
    Roller();
    virtual ~Roller();
    
    void RollInward();
    void RollOutward();
    void RollOpposite(int direction);

    virtual void Output();
};

#endif
