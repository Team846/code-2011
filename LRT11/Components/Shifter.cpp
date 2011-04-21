#include "Shifter.h"

Shifter::Shifter()
    : leftServo(RobotConfig::LEFT_GEARBOX_SERVO_PORT)
    , rightServo(RobotConfig::RIGHT_GEARBOX_SERVO_PORT)
    , encoders(DriveEncoders::GetInstance())
    , prefix("Shifter.")
    , currentGear(LOW_GEAR)
    , cycleCounter(0)
{

}

Shifter::~Shifter()
{

}

void Shifter::Configure()
{
    Config& config = Config::GetInstance();

    leftSetpointDeadband = config.Get<float>(prefix + "leftSetpointDeadband", 0.1);
    rightSetpointDeadband = config.Get<float>(prefix + "rightSetpointDeadband", 0.1);

    // default time of 2 seconds (100 cycles)
    forceShiftMs = config.Get<int>(prefix + "forceShiftMs", 2000);
}

void Shifter::Output()
{
#ifdef USE_DASHBOARD
//    SmartDashboard::Log(leftServo.Get(), "Left servo position");
//    SmartDashboard::Log(rightServo.Get(), "Right servo position");
#endif

//    DriverStation& station = *DriverStation::GetInstance();
//    leftServo.Set(station.GetAnalogIn(1));
//    rightServo.Set(station.GetAnalogIn(2));
//    return;

    if(action.shifter.force && !cycleCounter)
    {
        // ms * seconds / ms * cycles / second = cycles
        cycleCounter = (int)(forceShiftMs * 1.0 / 1000 * 50.0 / 1);
  //      cycleCounter = forceShiftMs / cycleMs;  -dg  (what I would do.  cycleMS computed in 'robot'
        action.shifter.force = false;
    }

    
    
    //If we are not moving, then don't power the servos, because they might not be able to engage. -dg
    	//Why not get the normalized speeds of each encoder?  -dg
    
// What Mr G might have written. -dg.    
//    boolean enableServo = false;
//    if (cycleCounter <= 0)  //if not shifting, then check if we are moving forward or turning
//    	enableServo = Util::Abs<float>( action.driveTrain.rate.rawForward)  > leftSetpointDeadband || 
//    	Util::Abs<float>( action.driveTrain.rate.rawTurn)  > leftSetpointDeadband );
//    
//	leftServo.SetEnabled(enableServo);
//	rightServo.SetEnabled(enableServo);
    
	
    bool forceShift = cycleCounter > 0;
    
    float leftSetpoint = action.driveTrain.rate.rawForward - action.driveTrain.rate.rawTurn;
    float rightSetpoint = action.driveTrain.rate.rawForward + action.driveTrain.rate.rawTurn;

    //Check the Left Servo
    if(Util::Abs<float>(leftSetpoint) > leftSetpointDeadband || forceShift)
        leftServo.SetEnabled(true);
    else
        leftServo.SetEnabled(false);
    
    
    //Now check the Right Servo
    if(Util::Abs<float>(rightSetpoint) > rightSetpointDeadband || forceShift)
        rightServo.SetEnabled(true);
    else
        rightServo.SetEnabled(false);

    
    
    
    switch(action.shifter.gear)
    {
    case LOW_GEAR:
        leftServo.Set(leftLowGearServoVal);
        rightServo.Set(rightLowGearServoVal);
        encoders.SetHighGear(false);
        break;

    case HIGH_GEAR:
        leftServo.Set(leftHighGearServoVal);
        rightServo.Set(rightHighGearServoVal);
        encoders.SetHighGear(true);
        break;
    }

    if(cycleCounter > 0)
        cycleCounter--;
}
