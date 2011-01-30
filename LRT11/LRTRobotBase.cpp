#include "LRTRobotBase.h"

/**
 * Constructor for RobotIterativeBase. Initializes member variables.
 */
LRTRobotBase::LRTRobotBase()
    : minCycleTime(1e6)
    , maxCycleTime(0)
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
bool LRTRobotBase::NextPeriodReady()
{
    return m_ds->IsNewControlData();
}

/**
 * Used to continuously call the MainLoop method while printing diagnostics.
 */
void LRTRobotBase::StartCompetition()
{
    // first and one-time initialization
    RobotInit();
    GetWatchdog().SetEnabled(false);

    const int reportPeriod = 50 * 5;
    double cycleWaitTimes[reportPeriod], cycleRunTimes[reportPeriod];

    int cycleIndex = 0;
    int packetsMissedInLastReportPeriod = 0;
    packetsMissedInLifetime = 0;

    Profiler &profiler = Profiler::GetInstance();
    UINT32 lastPacketNumber = 0;
    bool veryFirstPacketInLifetime = true;
    
    // buffer for the basic report period; see bottom of method
    char buffer[200];

    // loop forever, calling the main loop
    while(true)
    {
        GetWatchdog().Feed();

        double waitStart = Timer::GetFPGATimestamp();
        while(!NextPeriodReady())
            Wait(0.0005);   // allow other tasks to run

        // packet number used to determine if information was missed;
        // it increments by one for each new packet
        UINT32 dsPacketNumber = m_ds->GetPacketNumber();
        if(veryFirstPacketInLifetime)
            veryFirstPacketInLifetime = false;
        else
            packetsMissedInLastReportPeriod += dsPacketNumber - lastPacketNumber - 1;

        lastPacketNumber = dsPacketNumber;
        double cycleStart = Timer::GetFPGATimestamp();
        profiler.StartNewCycle();

        {
            ProfiledSection pf("MainLoop");
            MainLoop();
        }

        double cycleEnd = Timer::GetFPGATimestamp();
        double cycleRunTime = cycleEnd - cycleStart;
        double cycleWaitTime = cycleStart - waitStart;

        cycleRunTimes[cycleIndex] = cycleRunTime * 1000; // ms
        cycleWaitTimes[cycleIndex] = cycleWaitTime * 1000; // ms

        ++cycleIndex;
        if(cycleIndex == reportPeriod)
        {
            cycleIndex = 0;

            // reported statistics
            double runMin, runMax, runMean;
            double waitMin, waitMax, waitMean;

            Util::MinMaxMean<double>(cycleRunTimes, reportPeriod, &runMin, &runMax, &runMean);
            Util::MinMaxMean<double>(cycleWaitTimes, reportPeriod, &waitMin, &waitMax, &waitMean);

            packetsMissedInLifetime += packetsMissedInLastReportPeriod;
            sprintf( buffer, "Run time: [%.2f-%.2f]ms, ~%.2fms | Wait: [%.2f-%.2f]ms, ~%.2fms | Miss: %d pkts, (%d total)\n"
                    , runMin, runMax, runMean
                    , waitMin, waitMax, waitMean
                    , packetsMissedInLastReportPeriod, packetsMissedInLifetime );
            SmartDashboard::Log( buffer, "Basic Report" );
            packetsMissedInLastReportPeriod = 0;

            // minimum/maximum of all cycles
            minCycleTime = min(minCycleTime, runMin);
            maxCycleTime = max(maxCycleTime, runMax);
        }
    }
}
