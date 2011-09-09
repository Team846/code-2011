



#ifndef ACTION_DATA_H_
#define ACTION_DATA_H_

#include "General.h"

namespace ACTION
{
    //These status results are shared by many of the automated routines.
    enum eCompletionStatus
    { UNSET = 0, IN_PROGRESS = 1, SUCCESS = 2, FAILURE = 3, ABORTED = 4};
    char* const status_string[4 + 1] =
    {   "ERROR", "In Progress", "Success", "Failure", "Aborted" };



    //Define the states used by the ARM
    namespace ARM_ //Can't use ARM without the underscore.  Name conflict. Don't know why. -dg
    {
        enum eStates
        {
            IDLE = 1, PRESET_BOTTOM = 2, PRESET_TOP = 3,
            PRESET_MIDDLE = 4,  MANUAL_DOWN = 5, MANUAL_UP = 6
        };
        char* const state_string[6 + 1] =
        {"ERROR", "IDLE", "Preset_Bottom", "Preset_Top", "Preset_Middle", "Manual_Down", "Manual_Up"};
    }

    namespace LIFT
    {
        enum ePresets {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4};
        char* const presetStrings[4 + 1] =
        {   "ERROR", "Stowed", "Low Peg", "Med Peg", "Hi Peg" };

    }
    namespace DRIVETRAIN
    {
        enum eMode
        {
            SPEED = 0,
            POSITION = 1,
            DISTANCE = 2,
            SYNCHRONIZING = 3
        };
        char* const mode_strings[4] = {"RATE", "POSITION", "DISTANCE", "SYNCHRONIZING"};
    }

    namespace ROLLER
    {
        enum eStates {STOPPED = 1, SUCKING = 2, SPITTING = 3, ROTATING = 4};
        char* const state_string[6 + 1] =
        {"ERROR", "Stopped", "Sucking", "Spitting", "Rotating"};
    }

    namespace RINGER
    {
        enum eRingerStates
        {
            ARM_MIDDLE_POSITON = 0,
            COMMENCE_DROP_RINGER = 1,
            DROP_RINGER,
            TERMINATE_DROP_RINGER,
            ARM_UP,
            LIFT_DOWN,
            IDLE = 6
        };
        char* const state_string[6 + 1] =
        {
            "Arm Middle Position", "Comence Drop Ringer", "Drop Ringer",
            "Terminate_Drop_Ringer", "Arm Up", "Lift Down", "Idle"
        };
    }

    namespace GEARBOX
    {
        enum eGearboxState {LOW_GEAR = 1, HIGH_GEAR = 2};
        char* const state_string[2 + 1] = { "Low Gear", "High Gear"};

    }

} //namespace ACTION

struct ArmAction;
struct LiftAction;
struct DriveAction;

struct RollerAction;
struct DeployerAction;
struct ShifterAction;
struct ConfigAction;

class ActionData
{
private:
    static ActionData* instance;

public:
    ~ActionData();
    static ActionData& GetInstance();

    // master struct
    struct
    {
        bool abort;
    } master;

    DriveAction* driveTrain;
    LiftAction* lift;
    ArmAction* arm;
    RollerAction* roller;
    DeployerAction* deployer;
    ShifterAction* shifter;
    ConfigAction* config;

    struct
    {
        float power;
    } demoLift;


    struct
    {
        ACTION::RINGER::eRingerStates ringer;
    } automatedRoutine;


    struct
    {
        bool shouldCollect;
    } encoderData;

    bool wasDisabled;

protected:
    ActionData();
    DISALLOW_COPY_AND_ASSIGN(ActionData);
};

#endif
