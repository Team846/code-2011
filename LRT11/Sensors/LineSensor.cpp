#include "LineSensor.h"
#include <string>
#include "..\Util\Util.h"
#include <fstream>
#include "..\Util\AsynchronousPrinter.h"
#include <iomanip>
#include "..\Util\Profiler.h"

#define DefDelay() do { int i = 1; i--; } while(0)

LineSensor::LineSensor(int adcPort, int clockOut, int siOut)
    : analogIn(adcPort)
    , clockOut(clockOut)
    , siOut(siOut)
    , config(Config::GetInstance())
    , lastLinePos(0)
    , firstRun(true)
{
    const static string prefix = "LineSensor.";
    exposure = config.Get<int>(prefix + "exposure");
    exposure = 2;

    noiseThreshold = config.Get<int>(prefix + "noiseThreshold");
//    noiseThreshold = 150;
    noiseThreshold = 200;
    lineThreshold = config.Get<int>(prefix + "lineThreshold");
    lineThreshold = 200;
//    lineThreshold = 250;

    lineThreshold -= noiseThreshold;

//    UpdateReadings(); // throw away first reading
    Flush();
}

LineSensor::~LineSensor()
{
    free(pixelData); //cleanup
}

void LineSensor::UpdateReadings()
{
    ResetSensor();
    DelayMilliSeconds(exposure); //exposure is in milliseconds

    PrepFrame();

    for(int i = 0; i < NUM_PIXELS; i++)
    {
        Delay(50 * 10);
//        DelayMicroSeconds(1); //wait a microsecond

        pixelData[i] = analogIn.GetValue(); //if signal is noisy change to analogIn.GetAverageValue();

        if(firstRun)
            pixelData[i] = 0;

        //clock

        clockOut.Set(0);
//        DelayMicroSeconds(1); //wait a microsecond
        Delay(50 * 10);
        clockOut.Set(1);
    }

//    DelayMicroSeconds(1); //wait a microsecond
    Delay(50 * 10);
    clockOut.Set(0);
    Delay(50 * 10);
    clockOut.Set(0);
    Delay(50 * 10);
//    DelayMicroSeconds(1); //wait a microsecond

    firstRun = false;
}

void LineSensor::ResetFirstRun()
{
    firstRun = true;
    lastLinePos = 0;
}

void LineSensor::Flush()
{
    for(int i = 0; i < NUM_PIXELS; i++)
        pixelData[i] = 0;
}

void LineSensor::PrepFrame()
{
    clockOut.Set(0);//set clock low
//    DelayMicroSeconds(1);// wait for the sensor to read the clock
    Delay(50 * 10);
    siOut.Set(1);//start a new frame
//    DelayMicroSeconds(1);
//    DefDelay();
    Delay(50 * 10);
    clockOut.Set(1);//toggle clock
    Delay(50 * 10);
//    DelayMicroSeconds(1);
    siOut.Set(0);//reset frame
//    DelayMicroSeconds(1);

//    DefDelay();
    Delay(50 * 10);
}

void LineSensor::ResetSensor()
{
    PrepFrame();

    //interate through all pixels
    for(int i = 0; i < NUM_PIXELS; i++)
    {
        // just clock, don't bother reading (all data is to be discarded b/c exposure unknown)
        clockOut.Set(0);
        Delay(50 * 10);
//        DelayMicroSeconds(1);
        clockOut.Set(1);
        Delay(50 * 10);
//        DelayMicroSeconds(1);
    }
}

float LineSensor::GetLinePosition()
{
    unsigned int sum = 0, avg = 0, value, max = 0;
    onLine = false;
    UpdateReadings();

    for(int i = 0; i < NUM_PIXELS; i++)
    {
//      value = pixelData[i];
        value = Util::Max<int>(pixelData[i] - noiseThreshold, 0); //try to compensate for noise
        if(pixelData[max] < pixelData[i])
            max = i;
        if(value > 0)  //why is this here with the above max statement, Robert? -BA
        {
//          SmartDashboard::Log(value, "Linsense");
            onLine = onLine || (value > lineThreshold);//if the line has been seen
            avg += value * i; //weight by thresholded value, better would be to weight based on values around this one as well (exponential decrease)
            sum += value; //add to total sum
        }
    }

    {
        ProfiledSection pf("Test timer");
//      DefDelay(1);
    }

//    SmartDashboard::Log((int)max, "Max Line Sensor Pixel Value");
//    SmartDashboard::Log((int)pixelData[max], "Max Line Sensor Value");

    /*    static int e=0;
            //AsynchronousPrinter::Printf("Hello\n");
        if(e == 500) {
            char a;
            ofstream out("/lineout.txt", ios::app);
            out<<"\nHi";
            out.close();
            AsynchronousPrinter::Printf("Done.\n");
        }
        e++;*/

//  static int e = 0;
//  if(e++%25 == 0) // update every half second
//  {
//      ofstream out("/lineout.csv", ios::app);
//      for(int i = 0; i< NUM_PIXELS; i+=2)
//      {
////            value = Util::Max<int>(pixelData[i] - noiseThreshold, 0); //try to compensate for noise
//          value = pixelData[i];
//          out<<setw(3)<<value<<",";
//      }
//      out<<endl;
//      out.close();
//  }

    // 0 pixels is clockwise

    if(!onLine)
    {
        if(lastLinePos == 0) // starts out at 0 for floating point
            return 0;
        else if(lastLinePos < 0)
            return -0.25;
        else
            return 0.25;
    }
    else
    {
        avg /= sum; //find the avg. in the range [0, NUM_PIXELS*10]
        lastLinePos = Util::Rescale<float>(avg, 0, NUM_PIXELS, -1.0, 1.0); //map from 1 to -1
        return lastLinePos;
    }
}

bool LineSensor::IsOnLine()
{
    return onLine;
}

void LineSensor::DelayMilliSeconds(UINT32 ms)
{
    taskDelay(ms);
    // do not use Wait(); its resolution is only 1ms
//  UINT32 expiration = GetFPGATime() + us + 1;
//  while( GetFPGATime() < expiration )
//      ; // wait
}

void LineSensor::Delay(UINT32 ticks)
{
    while(ticks > 0)
        ticks--;
}
