#ifndef CONFIG_H_
#define CONFIG_H_

#include "../General.h"
#include "Configurable.h"
#include "../Util/Util.h"
#include "../Util/Console.h"
#include "../Util/Profiler.h"
#include <string>
#include <fstream>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class Config
{
public:
    const static int kNumAnalogAssignable = 4;

    virtual ~Config() { }
    static Config& GetInstance();

    const static string CONFIG_FILE_PATH;

    bool Load();
    bool Save();

    float ScaleAssignableAnalogValue(float value, int analogIndex);
    void UpdateAssignableDials();

    template <typename T> T Get(string key);
    template <typename T> T Get(string key, T defaultValue);
    template <typename T> void Set(string key, T val);

    static vector<Configurable*> configurables;
    static void RegisterConfigurable(Configurable* configurable);
    static void ConfigureAll();
    void CheckForFileUpdates();

    void Output();
    virtual string GetName();

protected:
    Config();

protected:
    static Config* instance;
    DriverStation& ds;
    time_t configLastReadTime_;

    map<string, string> configData;
    map<string, string> tload(string path);


    string analogAssignments[kNumAnalogAssignable];
    float analogAssignmentScaleMin[kNumAnalogAssignable];
    float analogAssignmentScaleMax[kNumAnalogAssignable];

    void Log(string key, string oldval, string newval);

    static bool hasRun;
    string buildNumKey, runNumKey, buildTimeKey;
    DISALLOW_COPY_AND_ASSIGN(Config);
};

#endif
