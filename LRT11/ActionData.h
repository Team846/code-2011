#ifndef ACTION_DATA_H_
#define ACTION_DATA_H_

#include "General.h"

namespace ACTION
{
    enum eCompletionStatus
    { IN_PROGRESS = 1, SUCCESS = 2, FAILURE = 3, ABORTED = 4};
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
    }
    namespace LIFT
    {
        enum ePresets {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4};
    }
} //namespace ACTION

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

    // drive train
    struct
    {
        enum
        {
            RATE,
            POSITION,
            DISTANCE,
            SYNCHRONIZING
        } mode;

        struct
        {
            float rawForward, rawTurn;
            bool brakeLeft, brakeRight;
            bool usingClosedLoop;
            bool thirdGear;
        } rate;

        struct
        {
            bool givenCommand;
            bool shouldMoveDistance, shouldTurnAngle;

            float distanceSetPoint, turnSetPoint; // inches, degrees
            float maxFwdSpeed, maxTurnSpeed;

            bool done;
        } position;

        struct
        {
            bool givenCommand;

            float distanceDutyCycle;
            float distanceSetPoint;

            bool done;
        } distance;

    } driveTrain;

    struct
    {
        bool givenCommand;
        bool manualMode;
        float power;

        bool highColumn;
        ACTION::LIFT::ePresets lift_preset;
        ACTION::eCompletionStatus completion_status;
    } lift;

    struct
    {
        float power;
    } demoLift;

    struct
    {
        ACTION::ARM_::eStates state;
        ACTION::eCompletionStatus completion_status;
    } arm;

    struct
    {
        enum {STOPPED = 1, SUCKING = 2, SPITTING = 3, ROTATING = 4} state;

        // true to rotate upward, false to rotate downward;
        // only active in ROTATING state
        bool rotateUpward;

        // used to automate roller spitting (rotate + reverse roller)
        bool automated;
        bool commenceAutomation;

        float maxSuckPower;
    } roller;

    struct
    {
        enum
        {
            ARM_MIDDLE_POSITON,
            COMMENCE_DROP_RINGER,
            DROP_RINGER,
            TERMINATE_DROP_RINGER,
            ARM_UP,
            LIFT_DOWN,
            IDLE
        } ringer;
    } automatedRoutine;

    struct
    {
        bool shouldAlignerRelease;
        bool shouldDeployMinibot;
    } deployer;

    struct
    {
        bool shouldCollect;
    } encoderData;

    struct
    {
        enum {LOW_GEAR = 1, HIGH_GEAR = 2} gear;
        bool force;
    } shifter;

    struct
    {
        bool load;
        bool save;
        bool apply;
    } config;

    bool wasDisabled;

protected:
    ActionData();
    DISALLOW_COPY_AND_ASSIGN(ActionData);
};

#endif
