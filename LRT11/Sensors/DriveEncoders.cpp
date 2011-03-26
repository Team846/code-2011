#include "DriveEncoders.h"

DriveEncoders* DriveEncoders::instance = NULL;

DriveEncoders& DriveEncoders::GetInstance()
{
    if(instance == NULL)
        instance = new DriveEncoders();
    return *instance;
}

DriveEncoders::DriveEncoders()
    : encoderLeft(RobotConfig::DIO_ENCODER_LEFT_A,
            RobotConfig::DIO_ENCODER_LEFT_B)
#ifndef VIRTUAL
    , uselessEncoder(1, 2)
#endif
    , encoderRight(RobotConfig::DIO_ENCODER_RIGHT_A,
            RobotConfig::DIO_ENCODER_RIGHT_B)
{
    AddToSingletonList();

    // want to stay with ticks/second
    encoderLeft.SetDistancePerPulse(1);
    encoderRight.SetDistancePerPulse(1);

    encoderLeft.Start();
    encoderRight.Start();
}

DriveEncoders::~DriveEncoders()
{
    // nothing
}

double DriveEncoders::GetForwardSpeed()
{
    return (encoderLeft.GetRate() + encoderRight.GetRate()) / 2;
}

double DriveEncoders::GetNormalizedForwardSpeed()
{
    return Util::Clamp<double>(GetForwardSpeed() / MAX_ENCODER_RATE, -1.0, 1.0);
}

double DriveEncoders::GetNormalizedLowGearForwardSpeed()
{
    return Util::Clamp<double>(GetNormalizedForwardSpeed() * LOW_GEAR_MULTIPLIER, -1.0, 1.0);
}

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

double DriveEncoders::GetRobotDist()
{
    return (GetLeftWheelDist() + GetRightWheelDist()) / 2;
}

double DriveEncoders::GetTurnTicks()
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

double DriveEncoders::GetLeftWheelDist()
{
    // pusles / ( pulses / revolution ) * distancen / revolution = inch distance
    return encoderLeft.Get() / PULSES_PER_REVOLUTION * WHEEL_DIAMETER * PI;
}

double DriveEncoders::GetRightWheelDist()
{
    // see GetLeftWheelDist() for calculation explanation
    return encoderRight.Get() / PULSES_PER_REVOLUTION * WHEEL_DIAMETER * PI;
}

double DriveEncoders::GetLeftSpeed()
{
    return encoderLeft.GetRate();
}

double DriveEncoders::GetNormalizedLeftSpeed()
{
    return Util::Clamp<double>(encoderLeft.GetRate() / MAX_ENCODER_RATE, -1.0, 1.0);
}

double DriveEncoders::GetRightSpeed()
{
    return encoderRight.GetRate();
}
double DriveEncoders::GetNormalizedRightSpeed()
{
    return Util::Clamp<double>(encoderRight.GetRate() / MAX_ENCODER_RATE, -1.0, 1.0);
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
