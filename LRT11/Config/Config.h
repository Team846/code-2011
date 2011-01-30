#ifndef CONFIG_H_
#define CONFIG_H_

#include "../General.h"
#include "Configurable.h"
#include "../Util/Util.h"
#include "../Util/Console.h"
#include "../Util/Profiler.h"
#include "../Util/AsynchronousPrinter.h"
#include <string>
#include <fstream>
#include <map>

class Config : public SensorBase
{
public:
    const static int kNumAnalogAssignable = 4;

    virtual ~Config() { }
    static Config& GetInstance();

    bool Load(string path = "/LRTConfig11.txt");
    bool Save(string path = "/LRTConfig11.txt");

    float ScaleAssignableAnalogValue(float value, int analogIndex);
    void UpdateAssignableDials();

    template <typename T> T Get(string key);
    template <typename T> T Get(string key, T defaultValue);
    template <typename T> void Set(string key, T val);

    static vector<Configurable*> configurables;
    static void RegisterConfigurable(Configurable* configurable);
    static void ConfigureAll();

protected:
    Config();

private:
    static Config* instance;
    DISALLOW_COPY_AND_ASSIGN(Config);

    map<string, string> configData;
    map<string, string> tload(string path);
    DriverStation& ds;

    string analogAssignments[kNumAnalogAssignable];
    float analogAssignmentScaleMin[kNumAnalogAssignable];
    float analogAssignmentScaleMax[kNumAnalogAssignable];

    void Log(string key, string oldval, string newval);

    static bool hasRun;
    string buildNumKey, runNumKey, buildTimeKey;
};

#endif
