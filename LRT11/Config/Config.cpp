#include "Config.h"
#include "Build.h"

Config* Config::instance = NULL;
vector<Configurable*> Config::configurables;
bool Config::hasRun = false;
const string Config::CONFIG_FILE_PATH = "/LRTConfig11.txt";
time_t Config::fileModifiedTime = 0;

Config& Config::GetInstance()
{
    if(instance == NULL)
        instance = new Config();
    return *instance;
}

Config::Config()
    : ds(*DriverStation::GetInstance())
    , buildNumKey("BuildNumber")
    , runNumKey("RunNumber")
    , buildTimeKey("BuildTime")
{
    for(int i = 0; i < kNumAnalogAssignable; ++i)
    {
        string keyname = "assignable." + Util::ToString<int>(i);
        analogAssignments[i] = "none";
        analogAssignmentScaleMin[i] = 0;
        analogAssignmentScaleMax[i] = 1;
    }

    CheckForFileUpdates();
    AddToSingletonList();
}

bool Config::Load()
{
    ProfiledSection pf("Config.Load");
    configData = tload(CONFIG_FILE_PATH);

    if(!hasRun)
    {
        if(Get<int>(buildNumKey, -1) == Build::GetNumber())
            Set<int>(runNumKey, Get<int>(runNumKey) + 1);
        else
        {
            Set<int>(runNumKey , 1);
            Set<int>(buildNumKey, Build::GetNumber());
        }

        Set<string>(buildTimeKey, Build::GetTime());

        Save();
        hasRun = true;
    }

    for(int i = 0; i < kNumAnalogAssignable; ++i)
    {
        string keyname = "assignable." + Util::ToString<int>(i);
        analogAssignments[i] = Get<string>(keyname + ".name");
        analogAssignmentScaleMin[i] = Get<float>(keyname + ".scaleMin");
        analogAssignmentScaleMax[i] = Get<float>(keyname + ".scaleMax");
    }

    return true;
}

map<string, string> Config::tload(string path)
{
    map<string, string> ret;

    ifstream fin(path.c_str());
    if(!fin.is_open())    // if the file does not exist then return false
        return ret;

    string key, val;
    while(getline(fin, key, '=') && getline(fin, val, '\n'))
    {
        {
            ProfiledSection pf("Config.tload");
            ret[key] = val;
        }

        AsynchronousPrinter::Printf("Cfg:%s=%s\n", key.c_str(), val.c_str());
    }

    fin.close();
    return ret;
}

void Config::Log(string key, string oldval, string newval)
{
    ofstream log("/log.txt", ios::app);
    log << key << " changed from " << oldval << " to " << newval << '\n';
    log.close();
}

bool Config::Save()
{
    ofstream fout(CONFIG_FILE_PATH.c_str());
    if(!fout.is_open())    // could not create file in that path
        return false;

    for(map<string, string>::const_iterator it = configData.begin(); it
            != configData.end(); it++)
        fout << it->first << "=" << it->second << "\n";

    fout.close();
    return true;
}

template <typename T> T Config::Get(string key)
{
    stringstream strstream(configData[key]);
    T ret;
    strstream >> ret;

    // [dcl]: Cause default values to be set, in the case of blank parameters.
    Set(key, ret);

    return ret;
}
template float Config::Get<float>(string key);
template bool Config::Get<bool>(string key);
template double Config::Get<double>(string key);
template string Config::Get<string>(string key);
template int Config::Get<int>(string key);

template <typename T> T Config::Get(string key, T defaultValue)
{
    if(configData.find(key) == configData.end())
    {
        Set(key, defaultValue);
        return defaultValue;
    }

    return Get<T>(key);
}
template float Config::Get<float>(string key, float defaultValue);
template bool Config::Get<bool>(string key, bool defaultValue);
template double Config::Get<double>(string key, double defaultValue);
template string Config::Get<string>(string key, string defaultValue);
template int Config::Get<int>(string key, int defaultValue);

template <typename T> void Config::Set(string key, T val)
{
    configData[key] = Util::ToString<T>(val);
}
template void Config::Set<float>(string key, float val);
template void Config::Set<bool>(string key, bool val);
template void Config::Set<double>(string key, double val);
template void Config::Set<string>(string key, string val);
template void Config::Set<int>(string key, int val);

float Config::ScaleAssignableAnalogValue(float value, int analogIndex)
{
    return Util::Rescale<float>(value, 0, 5,
            analogAssignmentScaleMin[analogIndex], analogAssignmentScaleMax[analogIndex]);
}

void Config::UpdateAssignableDials()
{
    for(int i = 0; i < kNumAnalogAssignable; ++i)
    {
        if(analogAssignments[i].length() && analogAssignments[i] != "none")
        {
            ProfilerHelper pf;

            pf.Start("UpdateAssignable.Scale");
            float newValue = ScaleAssignableAnalogValue(ds.GetAnalogIn(i + 1)
                    , i);
            pf.Finish();

            pf.Start("UpdateAssignable.Print");
            Console::GetInstance().PrintMultipleTimesPerSecond(0.25, "Assgn:%s= %f\n",
                    analogAssignments[i].c_str(), newValue);
            pf.Finish();

            pf.Start("UpdateAssignable.Set");
            Set(analogAssignments[i], Util::ToString<float>(newValue));
            pf.Finish();
        }
    }
}

void Config::RegisterConfigurable(Configurable* configurable)
{
    configurables.push_back(configurable);
}

void Config::ConfigureAll()
{
    for(unsigned int i = 0; i < configurables.size(); i++)
        configurables.at(i)->Configure();
}

void Config::Output()
{
    if(action.config.load)
    {
        AsynchronousPrinter::Printf("Loading Configuration\n");
        Load();
    }
    if(action.config.save)
    {
        AsynchronousPrinter::Printf("Saving Configuration\n");
        Save();
    }
    if(action.config.apply)
    {
        AsynchronousPrinter::Printf("Applying Configuration\n");
        ConfigureAll();
    }
}

void Config::CheckForFileUpdates()
{
    struct stat statistics;
    stat(CONFIG_FILE_PATH.c_str(), &statistics);

    // reload when the file has been modified
    if(fileModifiedTime != statistics.st_mtime)
    {
        Load();

        // must configure the new values
        AsynchronousPrinter::Printf("Applying Configuration\n");
        ConfigureAll();

        // Load() sometimes saves the configuration, so get the
        // new modified time
        stat(CONFIG_FILE_PATH.c_str(), &statistics);
        fileModifiedTime = statistics.st_mtime;
    }
}
