#ifndef ACTION_DATA_H_
#define ACTION_DATA_H_

#include "General.h"

class ActionData : public SensorBase
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
            DISTANCE
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

        bool highRow;
        enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4} preset;
        enum {STALE = 1, SUCCESS = 2, FAILURE = 3, ABORTED = 4} doneState;
    } lift;

    struct
    {
        float power;
    } demoLift;

    struct
    {
        bool givenCommand;
        bool manualMode;
        bool manualUp;
        bool presetTop;
        enum {STALE = 1, SUCCESS = 2, FAILURE = 3, ABORTED = 4} doneState;
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
    } roller;

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

protected:
    ActionData();
    DISALLOW_COPY_AND_ASSIGN(ActionData);
};

#endif
