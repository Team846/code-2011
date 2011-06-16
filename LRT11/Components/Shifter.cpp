#include "Shifter.h"
#include "../Util/AsynchronousPrinter.h"
#include "..\Config\Config.h"
#include "Shifter\LRTServo.h"
#include "Shifter\VirtualLRTServo.h"
#include "..\Sensors\DriveEncoders.h"
#include "..\Config\RobotConfig.h"
#include "..\ActionData\DriveAction.h"
#include "..\ActionData\ShifterAction.h"


Shifter::Shifter()
    : Component()
    , encoders(DriveEncoders::GetInstance())
    , prefix("Shifter.")
{
#ifdef VIRTUAL
    leftServo = new VirtualLRTServo(RobotConfig::PWM::LEFT_GEARBOX_SERVO, "Left Shift Servo")
    rightServo = new VirtualLRTServo(RobotConfig::PWM::RIGHT_GEARBOX_SERVO, "Right Shift Servo")
#else
    leftServo  = new LRTServo(RobotConfig::PWM::LEFT_GEARBOX_SERVO, "Left Shift Servo");
    rightServo = new LRTServo(RobotConfig::PWM::RIGHT_GEARBOX_SERVO, "Right Shift Servo");
#endif
    puts("Constructed Shifter");
}

Shifter::~Shifter()
{
    delete leftServo;
    delete rightServo;

}

void Shifter::Configure()
{
//    Config& config = Config::GetInstance();
}

void Shifter::Output()
{
#ifdef USE_DASHBOARD
//    SmartDashboard::Log(leftServo->Get(), "Left servo position");
//    SmartDashboard::Log(rightServo->Get(), "Right servo position");
#endif

//    DriverStation& station = *DriverStation::GetInstance();
//    leftServo->Set(station.GetAnalogIn(1));
//    rightServo->Set(station.GetAnalogIn(2));
//    return;

    //Power down servos if robot is not moving for several seconds; governed by servoDisableTimer -dg
    if(servoDisableTimer > 0)
        servoDisableTimer--;

    const bool robotTryingToMove =
        (action.driveTrain->rate.rawForward != 0.0 || action.driveTrain->rate.rawTurn != 0.0);

    if(robotTryingToMove || action.shifter->force)
        servoDisableTimer = kServoDisableDelay; //reset timer

    bool enableServo = servoDisableTimer > 0 ;

    leftServo->SetEnabled(enableServo);
    rightServo->SetEnabled(enableServo);

    switch(action.shifter->gear)
    {
    case ACTION::GEARBOX::LOW_GEAR:
        leftServo->Set(leftLowGearServoVal);
        rightServo->Set(rightLowGearServoVal);
        encoders.SetHighGear(false);
        break;

    case ACTION::GEARBOX::HIGH_GEAR:
        leftServo->Set(leftHighGearServoVal);
        rightServo->Set(rightHighGearServoVal);
        encoders.SetHighGear(true);
        break;

    default:
        AsynchronousPrinter::Printf("Fatal: %s:%d\n", __FILE__, __LINE__);
        break;
    }
}

string Shifter::GetName()
{
    return "Shifter";
}

