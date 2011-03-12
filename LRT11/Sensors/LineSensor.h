#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H
#include "..\Config\Configurable.h"
#include "..\Config\Config.h"
#include "..\General.h"

class LineSensor : public Configurable
{
public:
    LineSensor(int adcPort, int clockOut, int siOut);
    virtual ~LineSensor();

    void ResetFirstRun();
    float GetLinePosition();

    bool IsOnLine();
private:
    AnalogChannel analogIn;
    DigitalOutput clockOut, siOut;
    Config& config;

    const static int NUM_PIXELS = 128;
    float lastLinePos;
    unsigned int pixelData[NUM_PIXELS];
    unsigned int exposure, noiseThreshold, lineThreshold;
    bool firstRun;
    bool onLine;

    void UpdateReadings();
    void SetExpo(unsigned int exposure);
    void Flush();
    void ResetSensor();
    void PrepFrame();

    inline void DelayMilliSeconds(UINT32 ms);
    inline void Delay(UINT32 ticks);

};

#endif
