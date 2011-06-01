#include "LRTRobotBase.h"
#include "Utility.h"
#include "sysLib.h"
#include "Jaguar/ProxiedCANJaguar.h"

/**
 * Constructor for RobotIterativeBase. Initializes member variables.
 */
LRTRobotBase::LRTRobotBase()
    : quitting_(false)
//   , reader(JaguarReader::GetInstance())
    , cycleCount(0)

{
    printf("Creating LRTRobotbase\n");
}

/**
 * Free the resources for a RobotIterativeBase class.
 */
LRTRobotBase::~LRTRobotBase()
{
    printf("Deleting LRTRobotBase\n\n");    //should be our last access to the program.
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

    // must allow a negative value in case loop runs over 20ms
    INT32 sleepTime_us = 0;

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

    // loop until we are quitting -- must be set by the destructor of the derived class.
    while(!quitting_)
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
            AsynchronousPrinter::Printf("Sleep time: %.2fms\n\n", sleepTime_us * 1.0e-3);
//          printf("Time: %.4fms\n", GetFPGATime() * 1.0e-3);
//          printf("Target Time: %.4fms\n", cycleExpire_us * 1.0e-3);
//          fflush(stdout);
        }

        // implicit two's complement conversion allows sleeptime_us to be negative
        // even though both operands are UINT32s
        sleepTime_us = cycleExpire_us - GetFPGATime();

        //NB: This loop must be quit *before* the Jaguars are deleted!
        //TODO: This should be moved to "MainLoop()" -dg
        for(ProxiedCANJaguar* j = j->jaguar_list_; j != NULL; j = j->next_jaguar_)
            j->BeginComm(); //release semaphores.

//        for(int i = 0; i < ProxiedCANJaguar::jaguars.num; i++)
//           ProxiedCANJaguar::jaguars.j[i]->BeginComm();

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
