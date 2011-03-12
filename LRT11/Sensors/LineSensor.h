#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H
#include "..\Config\Configurable.h"
#include "..\Config\Config.h"
#include "..\General.h"
#include "LRTDigitalOutput.h"

class LineSensor
{
public:
    LineSensor(int adcPort, int clockOut, int siOut);
    virtual ~LineSensor();

    bool Read(int exposure_us);
    void ResetFirstRun();

    float GetLinePosition();
    bool IsLineDetected();

private:
    AnalogChannel adc;
    LRTDigitalOutput clock, si;

    const static int NUM_PIXELS = 128;
    float lastLinePos;

    unsigned int pixels[NUM_PIXELS + 1];
    unsigned int lineThreshold;

    bool firstRun;
    bool lineDetected;

    void ResetPixelData();

    inline void DelayMilliseconds(UINT32 ms);
    inline void Delay(UINT32 ticks);
};

#endif
