#include "ActionData.h"

ActionData* ActionData::instance = NULL;

ActionData& ActionData::GetInstance()
{
    if(instance == NULL)
        instance = new ActionData();
    return *instance;
}

ActionData::ActionData()
{
    AddToSingletonList();

    driveTrain.rawForward = 0;
    driveTrain.rawTurn = 0;
    driveTrain.brakeLeft = false;
    driveTrain.brakeRight = false;

    positionTrain.shouldMoveDistance = false;
    positionTrain.shouldTurnAngle = false;
    positionTrain.shouldOutputMoveDistance = false;
    positionTrain.shouldOutputTurnAngle = false;
    positionTrain.moveDistance = 0;
    positionTrain.turnAngle = 0;
    positionTrain.pivotLeft = false;
    positionTrain.pivotRight = false;
    positionTrain.done = false;
    positionTrain.enabled = false;

    lift.position = lift.STOWED;
    lift.highRow = false;
    lift.givenCommand = false;
    lift.manualMode = false;
    lift.power = 0;
    lift.done = false;

    demoLift.forward = 0;

    arm.givenCommand = false;
    arm.manualDown = false;
    arm.manualUp = false;
    arm.presetTop = false;
    arm.presetBottom = false;
    arm.position = arm.TOP;

    roller.state = roller.STOPPED;

    encoderData.shouldCollect = false;

    shifter.gear = shifter.LOW_GEAR;
    shifter.force = false;

    config.load = false;
    config.save = false;
    config.apply = false;
}

ActionData::~ActionData()
{

}
