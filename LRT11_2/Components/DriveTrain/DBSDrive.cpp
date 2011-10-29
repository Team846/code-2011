#include "DBSDrive.h"

DBSDrive::DBSDrive()
// TODO fix initialization
    : leftEncoder(DriveEncoders::GetInstance().GetLeftEncoder())
    , rightEncoder(DriveEncoders::GetInstance().GetRightEncoder())
    , driveEncoders(DriveEncoders::GetInstance())
{
    Configure();
}

DBSDrive::~DBSDrive()
{
    // nothing
}

void DBSDrive::Configure()
{
//    Config& config = Config::GetInstance();
//    string prefix = "DitheredBrakeDrive.";

    // proportional constant used to determine amount of braking
    // brake gain * ( joystick input - robot speed ) = desired braking
//    brakeGain = config.Get<float>(prefix + "brakeGain", 1.0);
}

// linearizes speed controller - motor response
ESCCommand DBSDrive::CalculateBrakeAndDutyCycle(float input, float speed)
{
    ESCCommand command;

    command.dutyCycle = 0.0;
    command.brakingDutyCycle = 0.0;

    if(speed < 0)
    {
        command = CalculateBrakeAndDutyCycle(-input, -speed);
        command.dutyCycle = -command.dutyCycle;
        return command;
    }

    // speed >= 0 at this point
    if(input >= speed) // trying to go faster
    {
        command.dutyCycle = input;
        command.brakingDutyCycle = 0.0;
    }
    // trying to slow down
    else
    {
        float error = input - speed; // error always <= 0

        if(input >= 0) // braking is based on speed alone; reverse power unnecessary
        {
            command.dutyCycle = 0.0; // must set 0 to brake

            if(speed > -error)
                command.brakingDutyCycle = -error / speed; // speed always > 0
            else
                command.brakingDutyCycle = 1.0;
        }
        else // input < 0; braking with reverse power
        {
            command.brakingDutyCycle = 0.0; // not braking
            command.dutyCycle = error / (1.0 + speed); // dutyCycle <= 0 because error <= 0
        }
    }

    return command;
}

DriveCommand DBSDrive::Drive(float forwardInput, float turnInput)
{
    DriveCommand drive;

    float leftInput = forwardInput - turnInput;
    float rightInput = forwardInput + turnInput;

    // TODO must decide whether forward or turn takes precedence
    if(Util::Abs<float>(leftInput) > 1.0)
        leftInput = Util::Sign<float>(leftInput) * 1.0;

    // TODO must decide whether forward or turn takes precedence
    if(Util::Abs<float>(rightInput) > 1.0)
        rightInput = Util::Sign<float>(rightInput) * 1.0;

    float leftSpeed  = driveEncoders.NormalizedMotorSpeed(leftEncoder);
    float rightSpeed = driveEncoders.NormalizedMotorSpeed(rightEncoder);;

    drive.leftCommand 	= CalculateBrakeAndDutyCycle(leftInput, leftSpeed);
    drive.rightCommand 	= CalculateBrakeAndDutyCycle(rightInput, rightSpeed);
    drive.shouldBrake	= true;

    return drive;
}
