#include "LRTRobotBase.h"
#include "Utility.h"
#include "sysLib.h"

/**
 * Constructor for RobotIterativeBase. Initializes member variables.
 */
LRTRobotBase::LRTRobotBase()
    : reader(JaguarReader::GetInstance())
    , cycleCount(0)

{

}

/**
 * Free the resources for a RobotIterativeBase class.
 */
LRTRobotBase::~LRTRobotBase()
{

}

/**
 * Returns true if new driver station data is available
 */
//bool LRTRobotBase::NextPeriodReady()
//{
//    return m_ds->IsNewControlData();
//}

/**
 * Used to continuously call the MainLoop method while printing diagnostics.
 */
void LRTRobotBase::StartCompetition()
{
    GetWatchdog().SetEnabled(false);

    // first and one-time initialization
    RobotInit();
    Profiler& profiler = Profiler::GetInstance();

    UINT32 sleepTime_us = 0;

//    const int reportPeriod = 50 * 5;
//    double cycleWaitTimes[reportPeriod], cycleRunTimes[reportPeriod];
//
//    int cycleIndex = 0;
//    int packetsMissedInLastReportPeriod = 0;
//    packetsMissedInLifetime = 0;

//    UINT32 lastPacketNumber = 0;
//    bool veryFirstPacketInLifetime = true;
//
//    // buffer for the basic report period; see bottom of method
//    char buffer[200];

    // loop forever, calling the main loop
    while(true)
    {
        cycleCount++;

        // GetFPGATime() is in microseconds
        // find the start of the next twenty millisecond tick of the clock
        UINT32 cycleExpire_us = ((UINT32)(GetFPGATime() / 20000) + 1) * 20000;
        profiler.StartNewCycle();

        {
            ProfiledSection ps("Main Loop");
            MainLoop();
        }

        if(cycleCount % 100 == 0)
        {
//          printf("Cycle count: %d\n", cycleCount);
            printf("Sleep time: %.2fms\n\n", sleepTime_us * 1.0e-3);
//          printf("Time: %.4fms\n", GetFPGATime() * 1.0e-3);
//          printf("Target Time: %.4fms\n", cycleExpire_us * 1.0e-3);
//          fflush(stdout);
        }

        sleepTime_us = cycleExpire_us - GetFPGATime();

//        AsynchronousPrinter::Printf("----\n");
        for(int i = 0; i < ProxiedCANJaguar::jaguars.num; i++)
            ProxiedCANJaguar::jaguars.j[i]->BeginComm();

        // sleep allows other threads to run -KV/DG 4/2011
        if(sleepTime_us > 0)
        {
//            reader.StartReading();
            taskDelay((UINT32)(sysClkRateGet() * sleepTime_us * 1.0e-6));
        }

        // sleep often returns early -KV/DG 4/2011
        while(GetFPGATime() < cycleExpire_us)
            ; // burn off cycles until target time has passed
    }
}
