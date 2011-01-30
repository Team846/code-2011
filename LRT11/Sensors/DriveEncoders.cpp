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
    , encoderRight(RobotConfig::DIO_ENCODER_RIGHT_A,
            RobotConfig::DIO_ENCODER_RIGHT_B)
{
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
    return Util::Clamp<double>(GetForwardSpeed() / kMaxEncoderRate, -1.0, 1.0);
}

double DriveEncoders::GetTurningSpeed()
{
    // WPILib GetRate error still exists with LRTEncoder?
    return encoderRight.GetRate() - encoderLeft.GetRate();
}

double DriveEncoders::GetNormalizedTurningSpeed()
{
    return GetTurningSpeed() / kMaxTurningRate;
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
    return (encoderRight.Get() - encoderLeft.Get()) / kTicksPerFullTurn;
}

double DriveEncoders::GetLeftWheelDist()
{
    // ( pulses / second ) / ( pulses / revolution )
    // * ( circumference / revolution ) = centimeter distance
    return encoderLeft.Get() / kPulsesPerRevolution * kWheelDiameter * kPi;
}

double DriveEncoders::GetRightWheelDist()
{
    // see GetLeftWheelDist() for calculation explanation
    return encoderRight.Get() / kPulsesPerRevolution * kWheelDiameter * kPi;
}

double DriveEncoders::GetNormalizedLeftSpeed()
{
    return Util::Clamp<double>(encoderLeft.GetRate() / kMaxEncoderRate, -1.0, 1.0);
}

double DriveEncoders::GetNormalizedRightSpeed()
{
    return Util::Clamp<double>(encoderRight.GetRate() / kMaxEncoderRate, -1.0, 1.0);
}

LRTEncoder& DriveEncoders::GetLeftEncoder()
{
    return encoderLeft;
}

LRTEncoder& DriveEncoders::GetRightEncoder()
{
    return encoderRight;
}
