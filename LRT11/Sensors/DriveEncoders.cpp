#include "DriveEncoders.h"

DriveEncoders* DriveEncoders::instance = NULL;

DriveEncoders& DriveEncoders::GetInstance()
{
    if(instance == NULL)
        instance = new DriveEncoders();
    return *instance;
}

DriveEncoders::DriveEncoders()
    : encoderLeft(
        RobotConfig::DIGITAL_IO::ENCODER_LEFT_A,
        RobotConfig::DIGITAL_IO::ENCODER_LEFT_B /*, 534.0 / 574.0 */)
#ifndef VIRTUAL
    , uselessEncoder(3, 6)
#endif
    , encoderRight(
        RobotConfig::DIGITAL_IO::ENCODER_RIGHT_A,
        RobotConfig::DIGITAL_IO::ENCODER_RIGHT_B)
    , isHighGear(false)
{
    AddToSingletonList();

    // want to stay with ticks/second
    encoderLeft.SetDistancePerPulse(1);
    encoderRight.SetDistancePerPulse(1);

    encoderLeft.Start();
    encoderRight.Start();
}

DriveEncoders::~DriveEncoders() {}


double DriveEncoders::GetForwardSpeed()
{
    return (encoderLeft.GetRate() + encoderRight.GetRate()) / 2;
}

//removed the clamping -BA, -DG 4/20/11
double DriveEncoders::GetNormalizedForwardSpeed()
{
    return GetForwardSpeed() / MAX_ENCODER_RATE;
}

double DriveEncoders::GetNormalizedLowGearForwardSpeed()
{
    return GetNormalizedForwardSpeed() * LOW_GEAR_MULTIPLIER;
}

double DriveEncoders::GetNormalizedForwardMotorSpeed()
{
    return isHighGear ? GetNormalizedForwardSpeed() :
            GetNormalizedLowGearForwardSpeed();
}

/***************** Turning Functions ***************************/
double DriveEncoders::GetNormalizedLowGearTurningSpeed()
{
    return GetNormalizedTurningSpeed() * LOW_GEAR_MULTIPLIER;
}

double DriveEncoders::GetTurningSpeed()
{
    // WPILib GetRate error still exists with LRTEncoder?
    return encoderRight.GetRate() - encoderLeft.GetRate();
}

double DriveEncoders::GetNormalizedTurningSpeed()
{
    return GetTurningSpeed() / MAX_TURNING_RATE;
}

double DriveEncoders::GetNormalizedTurningMotorSpeed()
{
    return isHighGear ? GetNormalizedTurningSpeed() :
            GetNormalizedLowGearTurningSpeed();
}

double DriveEncoders::GetRobotDist()
{
    return (GetWheelDist(LEFT) + GetWheelDist(RIGHT)) / 2;
}

int DriveEncoders::GetTurnTicks()
{
    // CCW is positive, CW is negative
    return encoderRight.Get() - encoderLeft.Get();
}

double DriveEncoders::GetTurnRevolutions()
{
    return GetTurnTicks() / TICKS_PER_FULL_TURN;
}

double DriveEncoders::GetTurnAngle()
{
    return GetTurnRevolutions() * 360.0;
}

/************* Distance functions **************************************/
double DriveEncoders::GetWheelDist(int side)
{
    // pulses / ( pulses / revolution ) * distance / revolution = inch distance
    LRTEncoder& e = (side == LEFT ? encoderLeft : encoderRight);
    return e.Get() / PULSES_PER_REVOLUTION * WHEEL_DIAMETER * PI;
}

double DriveEncoders::GetLeftSpeed()
{
    return encoderLeft.GetRate();
}

double DriveEncoders::GetNormalizedLeftOppositeGearMotorSpeed()
{
    return Util::Clamp<double>(
            encoderLeft.GetRate() /
            (!isHighGear ? MAX_ENCODER_RATE : (MAX_ENCODER_RATE / LOW_GEAR_MULTIPLIER))
            , -1.0, 1.0);
}

double DriveEncoders::GetRightSpeed()
{
    return encoderRight.GetRate();
}

double DriveEncoders::GetNormalizedMotorSpeed(LRTEncoder& encoder)
{
    return encoder.GetRate() /
            (isHighGear ? MAX_ENCODER_RATE : (MAX_ENCODER_RATE / LOW_GEAR_MULTIPLIER));
}

double DriveEncoders::GetNormalizedRightOppositeGearMotorSpeed()
{
    return Util::Clamp<double>(
            encoderRight.GetRate() /
            (!isHighGear ? MAX_ENCODER_RATE : (MAX_ENCODER_RATE / LOW_GEAR_MULTIPLIER))
            , -1.0, 1.0);
}

void DriveEncoders::SetHighGear(bool isHighGear)
{
    this->isHighGear = isHighGear;
}

#ifdef VIRTUAL
VirtualLRTEncoder& DriveEncoders::GetLeftEncoder()
#else
LRTEncoder& DriveEncoders::GetLeftEncoder()
#endif
{
    return encoderLeft;
}

#ifdef VIRTUAL
VirtualLRTEncoder& DriveEncoders::GetRightEncoder()
#else
LRTEncoder& DriveEncoders::GetRightEncoder()
#endif
{
    return encoderRight;
}
