#ifndef ARM_H_
#define Arm_H_

#include "Component.h"
#include "..\Config\RobotConfig.h"
#include "..\Jaguar\Esc.h"

class Arm : public Component
{
protected:
    ProxiedCANJaguar armEsc;

public:
    Arm();
    virtual ~Arm();
    void ArmShiftUp();
    void ArmShiftDown();
    void AbortArm();

    int GetArmPosition();
    AnalogChannel& GetArmPot();
    SafeCANJaguar& GetSpeedController();


};
#endif