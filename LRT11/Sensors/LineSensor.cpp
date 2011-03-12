#include "LineSensor.h"
#include <string>
#include "..\Util\Util.h"
#include <fstream>
#include "..\Util\AsynchronousPrinter.h"
#include <iomanip>
#include "..\Util\Profiler.h"

LineSensor::LineSensor(int adcPort, int clockPort, int siPort)
    : adc(adcPort)
    , clock(clockPort)
    , si(siPort)
    , lastLinePos(0)
    , firstRun(true)
{
    const static string prefix = "LineSensor.";

    lineThreshold = 200;

    si.Set(0);
    clock.Set(0);

    ResetPixelData(); // set all pixel values to 0
}

LineSensor::~LineSensor()
{
    // nothing
}

void LineSensor::ResetPixelData()
{
    for(int i = 0; i < NUM_PIXELS; i++)
        pixels[i] = 0;
}

bool LineSensor::Read(int exposure_us)
{
    int clockCycle = 1;

    si.Set(1); // starts sensor reset for next 18 cycles and schedules integrator after
    clock.Set(1); // sensor reset starts here
    si.Set(0);
    clock.Set(0);

    // clock out to the 18th cycle
    while(++clockCycle <= 18)
    {
        clock.Set(1);
        clock.Set(0);
    }

    // microseconds
    UINT32 expireTime = GetFPGATime() + exposure_us;

    // clock out to the 128th cycle
    do
    {
        clock.Set(1);
        clock.Set(0);
    }
    while(++clockCycle <= 128);

    // unwanted data is clocked out; now wait for exposure to complete
    while(GetFPGATime() < expireTime)
        ; // wait

    bool isGoodReading = (GetFPGATime() < expireTime + exposure_us / 10);
    clockCycle = 1; // reset to read data

    // 129th clock cycle triggers tristate and ends cycle
    clock.Set(1);
    clock.Set(0);

    // 129th clock cycle requires a minimum 20 microsecond delay
    Delay(50 * 20);

    si.Set(1); // starts sensor reset for next 18 cycles and schedules integrator after
    clock.Set(1); // sensor reset starts here
    si.Set(0);
    clock.Set(0);
    pixels[clockCycle] = adc.GetValue();

    while(++clockCycle <= 128)
    {
        clock.Set(1);
        clock.Set(0);
        pixels[clockCycle] = adc.GetValue();
    }

    // on the 129th clock cycle, the output will go to tristate
    clock.Set(1);
    clock.Set(0);

    return isGoodReading;
}

void LineSensor::ResetFirstRun()
{
    firstRun = true;
    lastLinePos = 0;
}

float LineSensor::GetLinePosition()
{
    unsigned int sum = 0, avg = 0, value, max = 0;
    lineDetected = false;

    {
        ProfiledSection pf("Update Readings");
        Read(4000);
    }

    for(int i = 0; i < NUM_PIXELS; i++)
    {
//      value = pixelData[i];
        value = pixels[i]; //try to compensate for noise
        if(pixels[max] < value)
            max = i;
        if(value > 0)
        {
            lineDetected = lineDetected || (value > lineThreshold);//if the line has been seen
            avg += value * i; //weight by thresholded value, better would be to weight based on values around this one as well (exponential decrease)
            sum += value; //add to total sum
        }
    }

    SmartDashboard::Log((int)max, "Max Line Sensor Pixel Value");
    SmartDashboard::Log((int)pixels[max], "Max Line Sensor Value");

    // 0 pixels is clockwise

    float linePos;

    if(!lineDetected)
    {
        if(lastLinePos == 0) // starts out at 0 for floating point
            linePos = 0;
        else if(lastLinePos < 0)
            linePos = 0.25;
        else
            linePos = -0.25;
    }
    else
    {
        avg /= sum; //find the avg. in the range [0, NUM_PIXELS*10]
        lastLinePos = Util::Rescale<float>(avg, 0, NUM_PIXELS, -1.0, 1.0); //map from 1 to -1
        linePos = lastLinePos;
    }

    return linePos;
}

bool LineSensor::IsLineDetected()
{
    return lineDetected;
}

void LineSensor::DelayMilliseconds(UINT32 ms)
{
    // parameter to taskDelay is in ms
    taskDelay(ms);
}

void LineSensor::Delay(UINT32 ticks)
{
    while(ticks > 0)
        ticks--;
}
