#include "LineSensor.h"
#include "..\General.h"
#include <string>

LineSensor::LineSensor(int adcPort, int clockOut, int siOut)
    : analogIn(adcPort)
    , clockOut(clockOut)
    , siOut(siOut)
    , config(Config::GetInstance())
{
    const static string prefix = "LineSensor.";
    exposure = config.Get<int>(prefix + "exposure");

    noiseThreshold = config.Get<int>(prefix + "noiseThreshold");
    lineThreshold = config.Get<int>(prefix + "lineThreshold");

    Flush();
}

LineSensor::~LineSensor()
{
    free(pixelData); //cleanup
}

void LineSensor::UpdateReadings()
{
    ResetSensor();
    DelayMicroSeconds(exposure); //exposure is in microseconds

    PrepFrame();

    for(int i = 0; i < NUM_PIXELS; i++)
    {
        DelayMicroSeconds(1); //wait a microsecond

        pixelData[i] = analogIn.GetValue(); //if signal is noisy change to analogIn.GetAverageValue();

        //clock
        clockOut.Set(0);
        DelayMicroSeconds(1); //wait a microsecond
        clockOut.Set(1);
    }
}

void LineSensor::Flush()
{
    for(int i = 0; i < NUM_PIXELS; i++)
        pixelData[i] = 0;
}

void LineSensor::PrepFrame()
{
    clockOut.Set(0);//set clock low
    DelayMicroSeconds(1);// wait for the sensor to read the clock
    siOut.Set(1);//start a new frame
    clockOut.Set(1);//toggle clock
    siOut.Set(0);//reset frame
}

void LineSensor::ResetSensor()
{
    PrepFrame();

    //interate through all pixels
    for(int i = 0; i < NUM_PIXELS; i++)
    {
        // just clock, don't bother reading (all data is to be discarded b/c exposure unknown)
        clockOut.Set(0);
        DelayMicroSeconds(1);
        clockOut.Set(1);
    }
}

float LineSensor::GetLinePosition()
{
    unsigned int sum, avg, value;
    bool onLine = false;
    UpdateReadings();

    for(int i = 0; i < NUM_PIXELS; i++)
    {
        value = Util::Max<int>(pixelData[i] - noiseThreshold, 0); //try to compensate for noise

        if(value > 0)  //why is this here with the above max statement, Robert? -BA
        {
            onLine = onLine || (value > lineThreshold);//if the line has been seen
            avg += value * i * 10; //weight by thresholded value, better would be to weight based on values around this one as well (exponential decrease)
            sum += value; //add to total sum
        }
    }

    if(!onLine)
    {
        if(lastLinePos < 0)
            return -1.0;
        else
            return 1.0;
    }
    else
    {
        avg /= sum; //find the avg. in the range [0, NUM_PIXELS*10]
        lastLinePos = Util::Rescale<float>(avg, 0, NUM_PIXELS * 10, -1.0, 1.0); //map from 1 to -1
        return lastLinePos;
    }
}

void LineSensor::DelayMicroSeconds(int us)
{
    Wait(us * 1.0e-6);
}
