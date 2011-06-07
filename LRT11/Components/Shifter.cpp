#include "Shifter.h"
#include "../Util/AsynchronousPrinter.h"

Shifter::Shifter()
    : leftServo(RobotConfig::PWM::LEFT_GEARBOX_SERVO, "Left Shift Servo")
    , rightServo(RobotConfig::PWM::RIGHT_GEARBOX_SERVO, "Right Shift Servo")
    , encoders(DriveEncoders::GetInstance())
    , prefix("Shifter.")
{
    puts("Constructed Shifter");
}

Shifter::~Shifter()
{

}

void Shifter::Configure()
{
//    Config& config = Config::GetInstance();
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

    //Power down servos if robot is not moving for several seconds; governed by servoDisableTimer -dg
    if(servoDisableTimer > 0)
        servoDisableTimer--;

    const bool robotTryingToMove =
        (action.driveTrain.rate.rawForward != 0.0 || action.driveTrain.rate.rawTurn != 0.0);

    if(robotTryingToMove || action.shifter.force)
        servoDisableTimer = kServoDisableDelay; //reset timer

    bool enableServo = servoDisableTimer > 0 ;

    leftServo.SetEnabled(enableServo);
    rightServo.SetEnabled(enableServo);

    switch(action.shifter.gear)
    {
    case ACTION::GEARBOX::LOW_GEAR:
        leftServo.Set(leftLowGearServoVal);
        rightServo.Set(rightLowGearServoVal);
        encoders.SetHighGear(false);
        break;

    case ACTION::GEARBOX::HIGH_GEAR:
        leftServo.Set(leftHighGearServoVal);
        rightServo.Set(rightHighGearServoVal);
        encoders.SetHighGear(true);
        break;

    default:
        AsynchronousPrinter::Printf("Fatal: %s:%d\n", __FILE__, __LINE__);
        break;
    }
}
