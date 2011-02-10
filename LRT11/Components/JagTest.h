#ifndef JAG_TEST_H_
#define JAG_TEST_H_

#include "..\General.h"
#include "Component.h"
#include "..\Jaguar\ProxiedCANJaguar.h"

class JagTest : public Component
{
public:
    ProxiedCANJaguar esc;
    DriverStation& ds;

    JagTest()
        : esc(4)
        , ds(*DriverStation::GetInstance())
    {
        esc.SetControlMode(CANJaguar::kPosition);
        esc.SetPositionReference(CANJaguar::kPosRef_Potentiometer);

        esc.SetPotentiometerTurns(1);
        esc.SetPID(200, 0, 0);

//        esc.EnableControl(0.5);
//        esc.DisableControl();
        esc.EnableControl();
    }

    virtual ~JagTest()
    {

    }

    virtual void Output()
    {
        esc.Set(ds.GetAnalogIn(1));
        SmartDashboard::Log(esc.GetPosition(), "Pot Position");
    }

    /*
    void Set(double position)
    {
        Set(position - ref);
    }
    */
};

#endif // JAG_TEST_H_
