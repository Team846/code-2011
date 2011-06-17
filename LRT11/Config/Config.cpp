#include "Build.h"
#include "Config.h"
#include "../Util/AsynchronousPrinter.h"
#include <ctype.h>
#include <sstream>

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
        analogAssignmentKeys[i] = "";
        analogAssignmentScaleMin[i] = 0;
        analogAssignmentScaleMax[i] = 1;
    }

    CheckForFileUpdates();
    printf("Constructed Config\n");
}

Config::~Config()
{
    if(configFile != NULL)
        delete configFile;

    if(newConfigData != NULL)
        delete newConfigData;

    if(sections != NULL)
        delete sections;
}

void Config::Load()
{
    ProfiledSection pf("Config.Load");
    LoadFile(CONFIG_FILE_PATH);

    const static string configSection = "Build";
    if(!hasRun)
    {
        if(Get<int>(configSection, buildNumKey, -1) == Build::GetNumber())
            Set<int>(configSection, runNumKey, Get<int>(configSection, runNumKey, 0) + 1);
        else
        {
            Set<int>(configSection, runNumKey , 1);
            Set<int>(configSection, buildNumKey, Build::GetNumber());
        }

        Set<string>(configSection, buildTimeKey, Build::GetTime());

        Save();
        hasRun = true;
    }

    //deal with assignable dials
    for(int i = 0; i < kNumAnalogAssignable; ++i)
    {
        string keyname = Util::ToString<int>(i);
        analogAssignmentKeys[i] = Get<string>("Assignable" , keyname + ".name", "");
        analogAssignmentSections[i] = Get<string>("Assignable" , keyname + ".section", "");
        analogAssignmentScaleMin[i] = Get<float>("Assignable" , keyname + ".scaleMin", 1);
        analogAssignmentScaleMax[i] = Get<float>("Assignable" , keyname + ".scaleMax", 1);
    }
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

void Config::Save()
{
    SaveToFile(CONFIG_FILE_PATH);
//    ofstream fout(CONFIG_FILE_PATH.c_str());
//    if(!fout.is_open())    // could not create file in that path
//        return false;
//
//    for(map<string, string>::const_iterator it = configData.begin();
//            it != configData.end(); it++)
//        fout << it->first << "=" << it->second << "\n";
//
//    fout.close();
//    return true;
}

//template <typename T> T Config::Get(string key)
//{
//    stringstream strstream(configData[key]);
//    T ret;
//    strstream >> ret;
//
//    // [dcl]: Cause default values to be set, in the case of blank parameters.
//    Set(key, ret);
//
//    return ret;
//}
//template float Config::Get<float>(string key);
//template bool Config::Get<bool>(string key);
//template double Config::Get<double>(string key);
//template string Config::Get<string>(string key);
//template int Config::Get<int>(string key);
//
//template <typename T> T Config::Get(string key, T defaultValue)
//{
//    if(configData.find(key) == configData.end())
//    {
//        Set(key, defaultValue);
//        return defaultValue;
//    }
//
//    return Get<T>(key);
//}
//template float Config::Get<float>(string key, float defaultValue);
//template bool Config::Get<bool>(string key, bool defaultValue);
//template double Config::Get<double>(string key, double defaultValue);
//template string Config::Get<string>(string key, string defaultValue);
//template int Config::Get<int>(string key, int defaultValue);
//
//template <typename T> void Config::Set(string key, T val)
//{
//    configData[key] = Util::ToString<T>(val);
//}
//template void Config::Set<float>(string key, float val);
//template void Config::Set<bool>(string key, bool val);
//template void Config::Set<double>(string key, double val);
//template void Config::Set<string>(string key, string val);
//template void Config::Set<int>(string key, int val);

float Config::ScaleAssignableAnalogValue(float value, int analogIndex)
{
    return Util::Rescale<float>(value, 0, 5,
            analogAssignmentScaleMin[analogIndex], analogAssignmentScaleMax[analogIndex]);
}

void Config::UpdateAssignableDials()
{
    for(int i = 0; i < kNumAnalogAssignable; ++i)
    {
        if(analogAssignmentKeys[i].length() != 0)
        {
            ProfilerHelper pf;

            pf.Start("UpdateAssignable.Scale");
            float newValue = ScaleAssignableAnalogValue(ds.GetAnalogIn(i + 1) , i);
            pf.Finish();

            pf.Start("UpdateAssignable.Print");
            Console::GetInstance().PrintMultipleTimesPerSecond(0.25, "Assgn:%s %s= %f\n",
                    analogAssignmentSections[i].c_str(), analogAssignmentKeys[i].c_str(), newValue);
            pf.Finish();

            pf.Start("UpdateAssignable.Set");
//            Set(analogAssignments[i], Util::ToString<float>(newValue));
            Set<float>(analogAssignmentSections[i], analogAssignmentKeys[i], newValue);
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

//being post 2011 season updates
bool Config::ValueExists(string section, string key)
{
    return newConfigData->find(section) != newConfigData->end() && (*newConfigData)[section].find(key) != (*newConfigData)[section].end();
}

template float Config::Get<float>(string section, string key, float defaultValue);
template bool Config::Get<bool>(string section, string key, bool defaultValue);
template double Config::Get<double>(string section, string key, double defaultValue);
template string Config::Get<string>(string section, string key, string defaultValue);
template int Config::Get<int>(string section, string key, int defaultValue);

template <typename T> T Config::Get(string section, string key, T defaultValue)
{
    if(ValueExists(section, key))
    {
        stringstream sstream((*newConfigData)[section][key].val);
        T ret;
        sstream >> ret;

        return ret;
    }
    else
    {
        AsynchronousPrinter::Printf("Using defualt value for %s %s which is %s\n", section.c_str(), key.c_str(), Util::ToString<T>(defaultValue).c_str());
        Set(section , key, defaultValue);
        return defaultValue;
    }
}

template void Config::Set<bool>(string section, string key, bool val);
template void Config::Set<int>(string section, string key, int val);
template void Config::Set<float>(string section, string key, float val);
template void Config::Set<double>(string section, string key, double val);
template void Config::Set<string>(string section, string key, string val);

template <typename T> void Config::Set(string section, string key, T val)
{
    string newVal = Util::ToString<T>(val);

    //workaround to ancient version of gcc not supporting all features of iterators
    list<string>::iterator sectionLocation = (*sections)[section];
    //if the value doesn't yet exist just add it
    if(ValueExists(section, key)) // need to add in the value in such a way that preserves whitespace and comments
    {
        list<string>::iterator valueLocation = (*newConfigData)[section][key].positionInFile;
        string oldVal = (*newConfigData)[section][key].val;

        //the location of the first occurence of the old value after the equals sign
        unsigned int locationOfStartOfOldValue = valueLocation->find(oldVal, valueLocation->find('='));
        valueLocation->replace(locationOfStartOfOldValue, oldVal.size(), newVal);
    }
    else // if value doesn't yet exist adding it in is easy
        configFile->insert(++sectionLocation, key + "=" + newVal);

    (*newConfigData)[section][key].val = newVal;
}

string trimWhiteSpace(string str)
{
    int startIndex = -1, endIndex = str.size() - 1;

    //find end of proceeding whitespace
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(!isspace(str[i]))
        {
            startIndex = i;
            break;
        }
    }

    if(startIndex == -1)
        return ""; //this line is all whitespace

    //Find end of trailing whitespace
    for(int i = str.size() - 1; i >= 0; i--)
    {
        if(!isspace(str[i]))
        {
            endIndex = i;
            break;
        }
    }

    return str.substr(startIndex, endIndex - startIndex + 1); //trim proceeding and trailing whitespace
}

//Tested in windows 6/16/11 -BA
void Config::LoadFile(string path)
{
    ProfiledSection pf("Config.parse+load");

    ifstream fin(path.c_str());
    if(!fin.is_open())
    {
        AsynchronousPrinter::Printf("Config could not open %s for reading\n", path.c_str());
        return;
    }

    //loading a the file discards all changes
    if(configFile != NULL)
        delete configFile;
    configFile = new list<string>();

    if(newConfigData != NULL)
        delete newConfigData;
    newConfigData = new config();

    if(sections != NULL)
        delete sections;
    sections = new map<string, list<string>::iterator >();


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
        string line = trimWhiteSpace(iter->substr(0, length)); //string minus the comments

        if(line.size() == 0) //if line is all whitespace ignore it
            continue;

        //check if start of new section
        if(line[0] == '[')
        {
            currentSection = line.substr(1, line.find_last_of("]") - 1);
            (*sections)[currentSection] = iter;
            AsynchronousPrinter::Printf("%s\n", currentSection.c_str());
            continue;
        }

        stringstream sstream(line);
        string key, val;
        getline(sstream, key, '=');
        getline(sstream, val);
        if(val.find("=") != string::npos)//we don't allow more than 1 equals sign per line leave a comment stating that
            (*iter) += " ; more than 1 equals sign per line is illegal";

        ConfigVal newVal;
        newVal.val = val;
        newVal.positionInFile = iter;
        AsynchronousPrinter::Printf("\t%s=%s\n", key.c_str(), val.c_str());
        (*newConfigData)[currentSection][key] = newVal;
    }
    fin.close();
}

void Config::SaveToFile(string path)
{
    ofstream fout(path.c_str());
    if(!fout.is_open())
    {
        AsynchronousPrinter::Printf("Config could not open %s for writing\n", path.c_str());
        return;
    }

    for(list<string>::iterator iter = configFile->begin(); iter != configFile->end(); iter++)
        fout << *iter << '\n';

    fout.close();
}
