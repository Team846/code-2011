#ifndef CANBUS_TESTER_H
#define CANBUS_TESTER_H
#include "Component.h"

class CanBusTester : Component
{
private:
    LRTCANJaguar jaggie;
public:
    CanBusTester()
#warning "Changeme"
        : jaggie(99)
    {
        jaggie.StartReadingCurrent();
    }

    virtual ~CanBusTester()
    {

    }

    virtual void Output()
    {
        AsynchronousPrinter::Printf("curr: %5.2f\n", jaggie.GetMostRecentCurrent());
#warning "Since dealing with multiple threads must test that it works consistently"

        static int i = 0;
        static float setPoint = 0.5;
        if(++i % 50 == 0)
        {
            setPoint = -setPoint;
            jaggie.Set(setPoint);
        }
    }
};
#endif
