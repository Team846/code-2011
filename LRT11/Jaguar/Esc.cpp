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
#ifdef VIRTUAL
Esc::Esc(int channel, VirtualLRTEncoder& encoder, string name)
#else
Esc::Esc(int channel, LRTEncoder& encoder, string name)
#endif
    : ProxiedCANJaguar(channel, "ESC?")
    , CANJaguarBrake((*(ProxiedCANJaguar*)this))
    , hasPartner(false)
    , partner(NULL)
    , encoder(encoder)
    , name(name)
    , index(0)
    , stoppingIntegrator(0)
    , errorRunning(0)
{
}

#ifdef VIRTUAL
Esc::Esc(int channelA, int channelB, VirtualLRTEncoder& encoder, string name)
#else
Esc::Esc(int channelA, int channelB, LRTEncoder& encoder, string name)
#endif
    : ProxiedCANJaguar(channelA, "ESC A?")
    , CANJaguarBrake((*(ProxiedCANJaguar*)this))
    , hasPartner(true)
    , partner(new Esc(channelB, encoder, name + "B"))
    , encoder(encoder)
    , name(name + "A")
    , index(0)
    , stoppingIntegrator(0)
    , errorRunning(0)
{
    printf("Constructed ESC: %s\n", name.c_str());
}

Esc::~Esc()
{
    return;
    //for some reason I can't delete partner without causing a crash when the task ends. -dg
    if(partner)
    {
        delete partner;
        partner = NULL;
        hasPartner = false;
    }
}



void Esc::Configure()
{
    if(hasPartner)
        partner->Configure();

    string prefix("Esc.");
    pGain = Config::GetInstance().Get<float>(prefix + "pGain", 4.0);
}

float Esc::GetNormalizedSpeed()
{
    return encoder.GetRate() / DriveEncoders::MAX_ENCODER_RATE;
}

void Esc::Stop()
{
    if(hasPartner)
        partner->Stop();

    float RobotSpeed = GetNormalizedSpeed();
    if(Util::Abs<double>(RobotSpeed) > 0.3)
    {
        SetBrake(8);
        Set(0.0);
        return;
    }
    double error = 0.0 - RobotSpeed;
//    static float k = 1. / 2;
//    errorRunning *= k;
//    errorRunning += error;

    double correction = pGain * stoppingIntegrator.UpdateSum(error);


//    if(error < 0.01)
//        Set(0.0);

//    Set(errorRunning * pGain * (1 - k));
    Set(correction);
    SetBrake(8);
}

void Esc::Set(float speed)
{
    if(hasPartner)
        partner->Set(speed);

#ifdef USE_DASHBOARD
//    SmartDashboard::Log(speed, name.c_str());
#endif

    // no current limiting
    ProxiedCANJaguar::Set(Util::Clamp<float>(speed, -1.0, 1.0));
//    controller.Set(channel, Util::Clamp<float>(speed, -1.0, 1.0));
}

void Esc::ResetCache()
{
    ProxiedCANJaguar::ResetCache();
    if(hasPartner)
        partner->ResetCache();
}

void Esc::ApplyBrakes()
{
    if(hasPartner)
        partner->ApplyBrakes();

    CANJaguarBrake::ApplyBrakes();
}

void Esc::SetBrake(int brakeAmount)
{
    if(hasPartner)
        partner->SetBrake(brakeAmount);

    CANJaguarBrake::SetBrake(brakeAmount);
}
