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

    driveTrain.mode = driveTrain.RATE;

    driveTrain.rate.rawForward = 0;
    driveTrain.rate.rawTurn = 0;
    driveTrain.rate.brakeLeft = false;
    driveTrain.rate.brakeRight = false;
    driveTrain.rate.thirdGear = false;
    // closed loop should default to on
    driveTrain.rate.usingClosedLoop = true;

    driveTrain.position.givenCommand = false;
    driveTrain.position.shouldMoveDistance = false;
    driveTrain.position.shouldTurnAngle = false;
    driveTrain.position.turnSetPoint = 0.0;
    driveTrain.position.distanceSetPoint = 0.0;
    driveTrain.position.maxFwdSpeed = 1.0;
    driveTrain.position.maxTurnSpeed = 1.0;
    driveTrain.position.done = false;

    driveTrain.distance.givenCommand = false;
    driveTrain.distance.distanceDutyCycle = 0.0;
    driveTrain.distance.distanceSetPoint = 0.0;
    driveTrain.distance.done = false;

    wasDisabled = true;

//    positionTrain.shouldMoveDistance = false;
//    positionTrain.shouldTurnAngle = false;
//    positionTrain.shouldOutputMoveDistance = false;
//    positionTrain.shouldOutputTurnAngle = false;
//    positionTrain.moveDistance = 0;
//    positionTrain.turnAngle = 0;
//    positionTrain.pivotLeft = false;
//    positionTrain.pivotRight = false;
//    positionTrain.done = false;
//    positionTrain.enabled = false;
//    // closed loop should default to on
//    positionTrain.usingClosedLoop = true;

    lift.givenCommand = false;
    lift.manualMode = false;
    lift.power = 0;
    lift.highColumn = false;
    lift.preset = lift.STOWED;
    lift.doneState = lift.IN_PROGRESS;

    demoLift.power = 0;

    arm.state = arm.IDLE;
    arm.doneState = arm.IN_PROGRESS;

    roller.state = roller.STOPPED;
    // if in roller.ROTATING state, default to rotating upward
    roller.rotateUpward = true;
    roller.automated = false;
    roller.commenceAutomation = false;

    deployer.shouldAlignerRelease = false;
    deployer.shouldDeployMinibot = false;

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
