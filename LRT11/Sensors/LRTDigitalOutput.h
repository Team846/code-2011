#ifndef LRT_DIGITAL_OUTPUT_H_
#define LRT_DIGITAL_OUTPUT_H_

#include "WPILib.h"

class LRTDigitalOutput : public DigitalOutput
{
public:
    LRTDigitalOutput(UINT32 channel)
        : DigitalOutput(channel)
    {

    }

    void Set(UINT32 value)
    {
        DigitalOutput::Set(value);
        Delay(50 * 1); // TODO refine delay?
    }

    void Delay(UINT32 ticks)
    {
        while(ticks > 0)
            ticks--;
    }
};

#endif
