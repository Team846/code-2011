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

    lift.position = lift.STOWED;
    lift.highRow = false;

    arm.usePreset = false;
    arm.position = arm.HIGH;
    arm.customSetpoint = 0.5;

    encoderData.shouldCollect = false;

    gearbox.gear = gearbox.LOW_GEAR;

    config.load = false;
    config.save = false;
    config.apply = false;
}

ActionData::~ActionData()
{

}
