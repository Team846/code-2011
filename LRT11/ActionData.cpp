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

    // used to abort movements
    master.abort = false;

    driveTrain.rawForward = 0;
    driveTrain.rawTurn = 0;
    driveTrain.brakeLeft = false;
    driveTrain.brakeRight = false;
    // closed loop should default to on
    driveTrain.usingClosedLoop = true;

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
    // closed loop should default to on
    positionTrain.usingClosedLoop = true;

    lift.preset = lift.STOWED;
    lift.highRow = false;
    lift.givenCommand = false;
    lift.manualMode = false;
    lift.power = 0;
    lift.doneState = lift.STALE;

    demoLift.forward = 0;

    arm.givenCommand = false;
    arm.manualUp = false;
    arm.presetTop = false;
    arm.doneState = arm.STALE;

    roller.state = roller.STOPPED;
    // if in roller.ROTATING state, default to rotating upward
    roller.rotateUpward = true;

    deployer.shouldAlignerRelease = false;

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
