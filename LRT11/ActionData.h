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

    // drive train
    struct
    {
        float rawForward, rawTurn;
        bool brakeLeft, brakeRight;
        bool usingClosedLoop;
    } driveTrain;

    struct
    {
        bool shouldMoveDistance, shouldTurnAngle;
        bool shouldOutputMoveDistance, shouldOutputTurnAngle;
        // moveDistance in inches, turnAngle in degrees
        float moveDistance, turnAngle;
        bool pivotLeft, pivotRight;
        bool done;
        bool enabled;
        bool closedLoopEnabled;
    } positionTrain;

    struct
    {
        bool givenCommand;
        bool manualMode;
        float power;
        bool highRow;
        enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4} position;
        enum {STALE = 1, SUCCESS = 2, FAILURE = 3, ABORT = 4} doneState;
    } lift;

    struct
    {
        float forward;
    } demoLift;

    struct
    {
        bool givenCommand;
        bool manualMode;
        bool manualUp;
        bool presetTop;
        enum {STALE = 1, SUCCESS = 2, FAILURE = 3, ABORT = 4} doneState;
    } arm;

    struct
    {
        enum {STOPPED = 1, SUCKING = 2, SPITTING = 3, ROTATING = 4} state;

        // true to rotate upward, false to rotate downward;
        // only active in ROTATING state
        bool rotateUpward;
    } roller;

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
        bool releaseRinger;
    } automation;

    struct
    {
        bool load, save, apply;
    } config;

protected:
    ActionData();
    DISALLOW_COPY_AND_ASSIGN(ActionData);
};

#endif
