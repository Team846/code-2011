#ifndef CL_POSITION_DRIVE_TRAIN_H_
#define CL_POSITION_DRIVE_TRAIN_H_

#include "..\..\General.h"
#include "..\..\Sensors\DriveEncoders.h"
#include "..\..\Config\Config.h"
#include "..\..\Config\Configurable.h"
#include "..\..\Util\RunningSum.h"
#include "CLRateTrain.h"
#include "DitheredBrakeTrain.h"

typedef struct {
	DriveCommand drive;
	bool isDone;
}CLPositionCommand;

class CLPositionDriveTrain : public Configurable
{
public:
    CLPositionDriveTrain(CLRateTrain& train);

    virtual void Configure();

    DriveCommand Drive(float fwdSetPoint, float turnSetpoint, bool stop);
    CLPositionCommand DriveAtLeastDistance(float fwdSetPoint);
    
    void ResetFwd();
    void ResetTurn();
    
private:
    CLRateTrain& drive;
    DriveEncoders& encoders;

    float pGainFwd;
    float pGainFwdTurnCorrection;
    float pGainTurn;
    float pGainTurnFwdCorrection;

    float fwdDeadband;
    float turnDeadband;

    float zeroDistance, zeroBearing;
    
    struct {
    	float target;
    	float initialBearing;
    	bool isSetUp;
    	bool goingForward;
    }moveDistanceInfo;
};

#endif
