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
#include <list>

typedef struct ConfigVal
{
    string val;
    list<string>::iterator positionInFile;
};

class Config
{
public:
    const static int kNumAnalogAssignable = 4;

    virtual ~Config();
    static Config& GetInstance();

    bool Load();
    bool Save();

    float ScaleAssignableAnalogValue(float value, int analogIndex);
    void UpdateAssignableDials();

    template <typename T> T Get(string key);
    template <typename T> T Get(string key, T defaultValue);
    template <typename T> void Set(string key, T val);

    template <typename T> T Get(string section, string key, T defaultValue);
    template <typename T> void Set(string section, string key, T val);

    static void RegisterConfigurable(Configurable* configurable);
    static void ConfigureAll();
    void CheckForFileUpdates();

private:
    Config();
    static Config* instance;
    time_t configLastReadTime_;

    const static string CONFIG_FILE_PATH;
    const static string COMMENT_DELIMITERS;

    static vector<Configurable*> configurables;

    list<string> *configFile; // line by line list of the config file

    map<string, string> configData;
    typedef map<string, map<string, ConfigVal> > config;
    config* newConfigData;
    map<string, list<string>::iterator > *sections;

    bool ValueExists(string section, string key);

    map<string, string> tload(string path);
    void LoadFile(string path);
    void SaveToFile(string path);

    DriverStation& ds;
    string analogAssignments[kNumAnalogAssignable];
    float analogAssignmentScaleMin[kNumAnalogAssignable];
    float analogAssignmentScaleMax[kNumAnalogAssignable];

    static bool hasRun;
    string buildNumKey, runNumKey, buildTimeKey;

    DISALLOW_COPY_AND_ASSIGN(Config);
};


#endif
