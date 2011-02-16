#define MIN(a,b) ((a)>(b)?(b):(a))
#define MAX(a,b) ((a)<(b)?(b):(a))

#include "Esc.h"

// CurrentLimiter class
Esc::CurrentLimiter::CurrentLimiter()
    : timeExtended(0)
    , timeBurst(0)
    , coolExtended(0)
    , coolBurst(0)
{
}

float Esc::CurrentLimiter::Limit(float targetSpeed, float robotSpeed)
{
//    if(Util::Abs<float>(targetSpeed) < .001)
//        return 0.0; //dont defeat the dynamic braking
//
//    const static float kMaxConst = .55;
//    if(targetSpeed < 0)
//        return -Limit(-targetSpeed, - robotSpeed);
//
//    float voltage_normalized = DriverStation::GetInstance()->GetBatteryVoltage() / 12;
//    float voltageLim = kMaxConst / voltage_normalized;
    return targetSpeed;
}

// Esc Class
Esc::Esc(int channel, LRTEncoder& encoder, string name) :
    ProxiedCANJaguar(channel), CANJaguarBrake((*(ProxiedCANJaguar*)this)),
    encoder(encoder), name(name), index(0), errorRunning(0)
{
}

void Esc::Configure()
{
    string prefix("Esc.");
    pGain = Config::GetInstance().Get<float>(prefix + "pGain", 4.0);
}

float Esc::GetNormalizedSpeed()
{
    return encoder.GetRate() / DriveEncoders::kMaxEncoderRate;
}

void Esc::Stop()
{
    float RobotSpeed = GetNormalizedSpeed();
    if(Util::Abs<double>(RobotSpeed) > 0.3)
    {
        ApplyBrakes(8);
        Set(0.0);
        return;
    }
    double error = 0.0 - RobotSpeed;
    static float k = 1. / 2;
    errorRunning *= k;
    errorRunning += error;

    Set(errorRunning * pGain * (1 - k));
    ApplyBrakes(8);
}

void Esc::Set(float speed)
{
    // no current limiting
    ProxiedCANJaguar::Set(Util::Clamp<float>(speed, -1.0, 1.0));
//    controller.Set(channel, Util::Clamp<float>(speed, -1.0, 1.0));
}
