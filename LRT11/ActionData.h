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
        // moveDistance in inches, turnAngle in degrees
        float moveDistance, turnAngle;
        bool pivotLeft, pivotRight;
        bool done;
    } positionTrain;

    struct
    {
        enum {STOWED = 1, LOW_PEG = 2, MED_PEG = 3, HIGH_PEG = 4} position;
        bool highRow;
    } lift;

    struct
    {
        bool usePreset;
        enum {LOW = 1, HIGH = 2} position;
        float customSetpoint;
    } arm;

    struct
    {
        bool shouldCollect;
    } encoderData;

    struct
    {
        enum {LOW_GEAR = 1, HIGH_GEAR = 2} gear;
    } gearbox;

    struct
    {
        bool load, save, apply;
    } config;

protected:
    ActionData();
    DISALLOW_COPY_AND_ASSIGN(ActionData);
};

#endif
