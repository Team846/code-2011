#include "ActionData.h"

ActionData* ActionData::instance = NULL;

ActionData& ActionData::GetInstance()
{
    if(instance == NULL)
        instance = new ActionData();
    return *instance;
}

ActionData::ActionData()
	:driveTrain.rawForward(0),
    driveTrain.rawTurn(0),
	driveTrain.brakeLeft(false),
	driveTrain.brakeRight(false),
	positionTrain.shouldMoveDistance(false),
	positionTrain.shouldTurnAngle(false),
	positionTrain.shouldOutputMoveDistance(false),
	positionTrain.shouldOutputTurnAngle(false),
	positionTrain.moveDistance(0),
	positionTrain.turnAngle(0),
	positionTrain.pivotLeft(false),
	positionTrain.pivotRight(false),
	positionTrain.done(false),
	lift.highRow(false),
	arm.usePreset(false),
	arm.customSetpoint(LOW),
	encoderData.shouldCollect(false),
	config.load(false),
	config.save(false),
	config.apply(false)
{
    AddToSingletonList();
}

ActionData::~ActionData()
{

}
