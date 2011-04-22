#include "Shifter.h"
#include "../Util/AsynchronousPrinter.h"

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

    //Why have two separate Deadbands?
//    leftSetpointDeadband = config.Get<float>(prefix + "leftSetpointDeadband", 0.02);
//    rightSetpointDeadband = config.Get<float>(prefix + "rightSetpointDeadband", 0.02);

    // 50 is cycles/sec. (Should use constant -dg)
    kShiftTime = (int) (50 * config.Get<float>(prefix + "forceShiftSeconds", 1.5));
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

    if(cycleCounter > 0)	//decrement the shift counter
        cycleCounter--;
    
    if(action.shifter.force && cycleCounter<=0)
         cycleCounter = kShiftTime; //set counter.
 
    //Power down servos if robot is not moving for several seconds; governed by servoDisableTimer -dg
    if (servoDisableTimer > 0)
    	servoDisableTimer--;
    
    const bool robotTryingToMove =
    	( action.driveTrain.rate.rawForward != 0.0 || action.driveTrain.rate.rawTurn != 0.0 );
    
    if (robotTryingToMove || action.shifter.force)
    	servoDisableTimer = kServoDisableDelay; //reset timer
       
    bool enableServo = servoDisableTimer>0 ;
 
	leftServo.SetEnabled(enableServo);
	rightServo.SetEnabled(enableServo);
    
	
//    bool forceShift = cycleCounter > 0;
//    
//    float leftSetpoint = action.driveTrain.rate.rawForward - action.driveTrain.rate.rawTurn;
//    float rightSetpoint = action.driveTrain.rate.rawForward + action.driveTrain.rate.rawTurn;
//
//    //Check the Left Servo
//    if(Util::Abs<float>(leftSetpoint) > leftSetpointDeadband || forceShift)
//        leftServo.SetEnabled(true);
//    else
//        leftServo.SetEnabled(false);
//    
//    
//    //Now check the Right Servo
//    if(Util::Abs<float>(rightSetpoint) > rightSetpointDeadband || forceShift)
//        rightServo.SetEnabled(true);
//    else
//        rightServo.SetEnabled(false);

    
    
    
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
	    default:
	    	AsynchronousPrinter::Printf("Fatal: %s:%s\n", __FILE__, __LINE__);
    }

}
