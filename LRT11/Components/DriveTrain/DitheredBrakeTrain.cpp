#include "DitheredBrakeTrain.h"

DitheredBrakeTrain::DitheredBrakeTrain()
// TODO fix initialization
    : leftEncoder(DriveEncoders::GetInstance().GetLeftEncoder())
    , rightEncoder(DriveEncoders::GetInstance().GetRightEncoder())
{
    Configure();
}

DitheredBrakeTrain::~DitheredBrakeTrain()
{
    // nothing
}

void DitheredBrakeTrain::Configure()
{
    Config& config = Config::GetInstance();
    string prefix = "DitheredBrakeDrive.";

    // proportional constant used to determine amount of braking
    // brake gain * ( joystick input - robot speed ) = desired braking
    brakeGain = config.Get<float>(prefix + "brakeGain", 1.0);
}

DriveCommand DitheredBrakeTrain::Drive(float forwardInput, float turnInput)
{
    DriveCommand drive;

    // positive turn is clockwise
    drive.leftDC = forwardInput + turnInput;
    drive.rightDC = forwardInput - turnInput;

    if(Util::Abs<float>(drive.leftDC) > 1.0)
    {
        // TODO must decide whether forward or turn takes precedence
        drive.leftDC = Util::Sign<float>(drive.leftDC) * 1.0;
    }

    if(Util::Abs<float>(drive.rightDC) > 1.0)
    {
        // TODO must decide whether forward or turn takes precedence
        drive.rightDC = Util::Sign<float>(drive.rightDC) * 1.0;
    }

    // TODO make this based on low/high gear
    // normalize speeds
    float leftSpeed = leftEncoder.GetRate() / DriveEncoders::MAX_ENCODER_RATE;
    float rightSpeed = rightEncoder.GetRate() / DriveEncoders::MAX_ENCODER_RATE;

    // clamp speeds to [-1.0,1.0] interval
    leftSpeed = Util::Clamp<float>(leftSpeed, -1.0, 1.0);
    rightSpeed = Util::Clamp<float>(rightSpeed, -1.0, 1.0);

    // assume no braking is necessary
    drive.leftBrakeDC = 0.0;
    drive.rightBrakeDC = 0.0;

    // braking is necessary on the left side if leftSpeed > leftInput
    // signs must be taken into account when comparing
    // TODO add minimum difference to start braking
    if(Util::Abs<float>(rightSpeed) > 3e-2 && (Util::Abs<float>(leftSpeed) > Util::Abs<float>(drive.leftDC) ||
            Util::Sign<float>(leftSpeed) != Util::Sign<float>(drive.leftDC)))
    {
        float error = Util::Abs<float>(leftSpeed - drive.leftDC);

        // braking is proportional to the error
        float desiredBraking = brakeGain * error;

        // brake duty cycle is equal to desired stall torque / normalized speed
        drive.leftBrakeDC = desiredBraking / Util::Abs<float>(leftSpeed);
        drive.leftDC = 0.0; // can only brake if speed controller is set to 0
    }

    // braking is necessary on the right side if rightSpeed > rightInput
    // signs must be taken into account when comparing
    // TODO add minimum difference to start braking
    if(Util::Abs<float>(rightSpeed) > 3e-2 && (Util::Abs<float>(rightSpeed) > Util::Abs<float>(drive.rightDC) ||
            Util::Sign<float>(rightSpeed) != Util::Sign<float>(drive.rightDC)))
    {
        float error = Util::Abs<float>(rightSpeed - drive.rightDC);

        // braking is proportional to the error
        float desiredBraking = brakeGain * error;

        // brake duty cycle is equal to desired stall torque / normalized speed
        drive.rightBrakeDC = desiredBraking / Util::Abs<float>(rightSpeed);
        drive.rightDC = 0.0; // can only brake if speed controller is set to 0
    }

    return drive;
}
