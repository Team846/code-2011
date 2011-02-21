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
    } driveTrain;

    struct
    {
        bool shouldMoveDistance, shouldTurnAngle;
        bool shouldOutputMoveDistance, shouldOutputTurnAngle;
        // moveDistance in inches, turnAngle in degrees
        float moveDistance, turnAngle;
        bool pivotLeft, pivotRight;
        bool done;
    } positionTrain;

    struct
    {
        enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4} position;
        bool highRow;
        bool givenCommand;
    } lift;

    struct
    {
        float forward;
    } demoLift;

    struct
    {
        bool usePreset;
        enum {BOTTOM = 1, TOP = 2} position;
        float customSetpoint;
    } arm;

    struct
    {
        enum {STOPPED = 1, SUCKING = 2, SPITTING = 3, ROLLING = 4} state;
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
        bool load, save, apply;
    } config;

protected:
    ActionData();
    DISALLOW_COPY_AND_ASSIGN(ActionData);
};

#endif
