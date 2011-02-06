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
        enum {kStowed = 1, kLowPeg = 2, kMedPeg = 3, kHighPeg = 4} position;
        bool highRow;
    } lift;

    struct
    {
        bool preset;
        enum {kLow = 1, kHigh = 2} position;
        float customSetpoint;
    } arm;

    struct
    {
        bool shouldCollect;
    } encoderData;

    struct
    {
        enum {kLowGear = 1, kHighGear = 2} gear;
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
