#include "Build.h"
#include "Config.h"
#include "../Util/AsynchronousPrinter.h"
#include <ctype.h>

Config* Config::instance = NULL;
vector<Configurable*> Config::configurables;
bool Config::hasRun = false;
const string Config::CONFIG_FILE_PATH = "/LRTConfig11.txt";
const string Config::COMMENT_DELIMITERS = "#;";

Config& Config::GetInstance()
{
    if(instance == NULL)
        instance = new Config();
    return *instance;
}

Config::Config()
    : configLastReadTime_(0)
    , ds(*DriverStation::GetInstance())
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
    printf("Constructed Config\n");
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

    //deal with assignable dials
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

        AsynchronousPrinter::Printf("  Cfg:%s=%s\n", key.c_str(), val.c_str());
    }

    fin.close();
    return ret;
}

bool Config::Save()
{
    ofstream fout(CONFIG_FILE_PATH.c_str());
    if(!fout.is_open())    // could not create file in that path
        return false;

    for(map<string, string>::const_iterator it = configData.begin();
            it != configData.end(); it++)
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

/*
 * CheckForFileUpdates()
 * checks to see if the Robot's configuration file on the cRio has been modified since
 * it was last loaded.
 */
void Config::CheckForFileUpdates()
{
    struct stat statistics;
    stat(CONFIG_FILE_PATH.c_str(), &statistics);

    // reload when the file has been modified
    if(configLastReadTime_ != statistics.st_mtime)
    {
        Load();

        // must configure the new values
        AsynchronousPrinter::Printf("Applying Configuration\n");
        ConfigureAll();

        //Wait for printing if this is the first time the program is read.
        if(0 == configLastReadTime_)
            while(!AsynchronousPrinter::QueueEmpty())
                Wait(0.010);

        // Load() sometimes saves the configuration,
        // so get the new modified time.
        stat(CONFIG_FILE_PATH.c_str(), &statistics);
        configLastReadTime_ = statistics.st_mtime;
    }
}

void Config::loadFile(string path)
{
    ifstream fin(path.c_str());
    if(!fin.is_open())
    {
        AsynchronousPrinter::Printf(strcat("Config could not open ", path.c_str()));
        return;
    }
    //loading a the file discards all changes
    if(configFile != NULL)
        delete configFile;
    configFile = new list<string>();
    if(newConfigData != NULL)
        delete newConfigData;
    newConfigData = new config();

    //read the file into memory
    while(!fin.eof())
    {
        string line;
        getline(fin, line);
        configFile->push_back(line);
    }
    fin.close();

    //parse the file
    string currentSection;
    for(list<string>::iterator iter = configFile->begin(); iter != configFile->end(); iter++)
    {
        unsigned int length = iter->find_first_of(COMMENT_DELIMITERS.c_str());
        if(length == string::npos)
            length = iter->size();
        string line = iter->substr(0, length); //string minus the comments

        int startIndex = -1, endIndex = line.size() - 1;

        //find end of proceeding whitespace
        for(unsigned int i = 0; i < line.size(); i++)
        {
            if(!isspace(line[i]))
            {
                startIndex = i;
                break;
            }
        }

        if(startIndex == -1)
            continue; //this line is all whitespace

        //Find end of trailing whitespace
        for(int i = line.size() - 1; i >= 0; i--)
        {
            if(!isspace(line[i]))
            {
                endIndex = i;
                break;
            }
        }

        line = line.substr(startIndex, endIndex - startIndex + 1); //trim proceeding and trailing whitespace

        //check if start of new section
        if(line[0] == '[')
        {
            currentSection = line.substr(1, line.find_last_of("]"));
            continue;
        }



    }
}
