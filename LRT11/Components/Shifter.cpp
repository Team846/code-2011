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
//    leftShiftServo.Set(station.GetAnalogIn(1));
//    rightShiftServo.Set(station.GetAnalogIn(2));

    if(action.shifter.force && !cycleCounter)
    {
        // ms * seconds / ms * cycles / second = cycles
        cycleCounter = (int)(forceShiftMs * 1.0 / 1000 * 50.0 / 1);
        action.shifter.force = false;
    }

    bool forceShift = cycleCounter > 0;
    float leftSetpoint = action.driveTrain.rate.rawForward - action.driveTrain.rate.rawTurn;
    float rightSetpoint = action.driveTrain.rate.rawForward + action.driveTrain.rate.rawTurn;

    if(Util::Abs<float>(leftSetpoint) > leftSetpointDeadband || forceShift)
        leftServo.SetEnabled(true);
    else
        leftServo.SetEnabled(false);

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

    if(forceShift)
        cycleCounter--;
}
