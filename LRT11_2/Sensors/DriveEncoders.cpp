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
    , highGear(false)
{
    // want to stay with ticks/second
    encoderLeft.SetDistancePerPulse(1);
    encoderRight.SetDistancePerPulse(1);

    encoderLeft.Start();
    encoderRight.Start();
    printf("Construct Drive Encoders\n");
}

DriveEncoders::~DriveEncoders() {}


double DriveEncoders::RawForwardSpeed()
{
    return (encoderLeft.GetRate() + encoderRight.GetRate()) / 2;
}

double DriveEncoders::NormalizedForwardMotorSpeed()
{
	double forwardSpeed = RawForwardSpeed() / MAX_ENCODER_RATE;

	if (!highGear)
		forwardSpeed *= LOW_GEAR_MULTIPLIER;
	
	return forwardSpeed;
}

/***************** Turning Functions ***************************/
double DriveEncoders::NormalizedLowGearTurningSpeed()
{
    return NormalizedTurningSpeed() * LOW_GEAR_MULTIPLIER;
}

double DriveEncoders::TurningSpeed()
{
    // WPILib GetRate error still exists with LRTEncoder?
    return encoderRight.GetRate() - encoderLeft.GetRate();
}

double DriveEncoders::NormalizedTurningSpeed()
{
    return TurningSpeed() / MAX_TURNING_RATE;
}

double DriveEncoders::NormalizedTurningMotorSpeed()
{
    return highGear ? NormalizedTurningSpeed() :
            NormalizedLowGearTurningSpeed();
}

double DriveEncoders::RobotDist()
{
    return (WheelDist(LEFT) + WheelDist(RIGHT)) / 2;
}

int DriveEncoders::TurnTicks()
{
    // CCW is positive, CW is negative
    return encoderRight.Get() - encoderLeft.Get();
}

double DriveEncoders::TurnRevolutions()
{
    return TurnTicks() / TICKS_PER_FULL_TURN;
}

double DriveEncoders::TurnAngle()
{
    return TurnRevolutions() * 360.0;
}

/************* Distance functions **************************************/
double DriveEncoders::WheelDist(int side)
{
    // pulses / ( pulses / revolution ) * distance / revolution = inch distance
    LRTEncoder& e = (side == LEFT ? encoderLeft : encoderRight);
    return e.Get() / PULSES_PER_REVOLUTION * WHEEL_DIAMETER * PI;
}

double DriveEncoders::LeftSpeed()
{
    return encoderLeft.GetRate();
}

double DriveEncoders::NormalizedLeftOppositeGearMotorSpeed()
{
    return Util::Clamp<double>(
            encoderLeft.GetRate() /
            (!highGear ? MAX_ENCODER_RATE : (MAX_ENCODER_RATE / LOW_GEAR_MULTIPLIER))
            , -1.0, 1.0);
}

double DriveEncoders::RightSpeed()
{
    return encoderRight.GetRate();
}

double DriveEncoders::NormalizedMotorSpeed(LRTEncoder& encoder)
{
    return encoder.GetRate() /
            (highGear ? MAX_ENCODER_RATE : (MAX_ENCODER_RATE / LOW_GEAR_MULTIPLIER));
}

double DriveEncoders::NormalizedOpposingGearMotorSpeed(LRTEncoder& encoder)
{
    return encoder.GetRate() /
            (highGear ? (MAX_ENCODER_RATE / LOW_GEAR_MULTIPLIER) : MAX_ENCODER_RATE );
}

double DriveEncoders::NormalizedRightOppositeGearMotorSpeed()
{
    return encoderRight.GetRate() /
            (highGear ? (MAX_ENCODER_RATE / LOW_GEAR_MULTIPLIER) : MAX_ENCODER_RATE );
}

void DriveEncoders::SetHighGear(bool highGear)
{
    this->highGear = highGear;
}

bool DriveEncoders::isHighGear()
{
	return highGear;
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
