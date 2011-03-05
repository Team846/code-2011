#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H
#include "Component.h"
#include "..\Config\Configurable.h"
#include "..\Config\Config.h"

class LineSensor : public Component, public Configurable
{
public:
    LineSensor(int adcPort, int clockOut, int siOut);
    virtual ~LineSensor();

    float GetLinePosition();

private:
    AnalogChannel analogIn;
    DigitalOutput clockOut, siOut;
    Config& config;

    const static int NUM_PIXELS = 128;
    float lastLinePos;
    unsigned int pixelData[NUM_PIXELS];
    unsigned int exposure, noiseThreshold, lineThreshold;

    void UpdateReadings();
    void SetExpo(unsigned int exposure);
    void Flush();
    void ResetSensor();
    void PrepFrame();

    inline void DelayMicroSeconds(int us);
};

#endif
