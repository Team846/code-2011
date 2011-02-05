#include "Profiler.h"

Profiler* Profiler::instance = NULL;

Profiler& Profiler::GetInstance()
{
    if(instance == NULL)
        instance = new Profiler();
    return *instance;
}

Profiler::Profiler() :
    cycleIndex(0)
{
    AddToSingletonList();
}

template <class PairT> struct SortBySecondValue
{
    bool operator()(const PairT& lhs, const PairT& rhs) const
    {
        return lhs.second > rhs.second;
    }
};

void Profiler::StartNewCycle()
{
    ++cycleIndex;

    if(cycleIndex >= reportPeriod)
    {
        double reportStart = Timer::GetFPGATimestamp();
        stringstream data;

        data << "Profiler (" << reportPeriod << " cycles)\n";

        typedef map<string, double>::value_type paired;
        typedef set< paired , SortBySecondValue<paired> > SetSortedBySecond;
        SetSortedBySecond vals;

        for(map<string, double>::iterator it = loggedMaxs.begin(); it
                != loggedMaxs.end(); ++it)
        {
            // vals.insert( paired(it->first, it->second/loggedCounts[it->first]) ); // to sort by means
            vals.insert(paired(it->first, it->second));
        }

        int i = 0;
        for(SetSortedBySecond::iterator it = vals.begin(); it != vals.end(); ++it)
        {
            double min = loggedMins[it->first];
            double max = loggedMaxs[it->first];
            int count = loggedCounts[it->first];
            double mean = loggedSums[it->first] / count;

            data << " | " << left;
            data.width(30);

            data << it->first << right;
            data.width(-1);

            data << " ~" << fixed << setprecision(2) << mean << " [" << min << " - " << max <<
                    "]" << "x" << count << "\n";

            ++i;
            if(i > reportLimit)
                break;
        }

        double reportTime = (Timer::GetFPGATimestamp() - reportStart) * 1000;

        cycleIndex = 0;
        ClearLogBuffer();

        data << "Report took " << reportTime << "ms\n";
        SmartDashboard::Log(data.str().c_str(), "Detailed Report");
        // AsynchronousPrinter::Printf( data.str().c_str() );
    }
}

void Profiler::Log(std::string name, double timeTaken)
{
    if(loggedCounts.find(name) == loggedCounts.end())
    {
        loggedCounts[name] = 1;
        loggedMins[name] = loggedMaxs[name] = loggedSums[name]
                = timeTaken;
    }
    else
    {
        ++loggedCounts[name];
        if(timeTaken < loggedMins[name])
            loggedMins[name] = timeTaken;
        if(timeTaken > loggedMaxs[name])
            loggedMaxs[name] = timeTaken;
        loggedSums[name] += timeTaken;
    }
}

void Profiler::ClearLogBuffer()
{
    loggedCounts.clear();
    loggedMins.clear();
    loggedMaxs.clear();
    loggedSums.clear();
}
