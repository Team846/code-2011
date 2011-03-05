#include "VirtualLRTEncoder.h"
#include "DriveEncoders.h"

#define FPS_TO_TPS(fps) ((fps) * 12.0 * 1 / (4.0 * DriveEncoders::PI)* DriveEncoders::TICKS_PER_FULL_TURN)

double VirtualLRTEncoder::HIGH_GEAR_MAX_RATE = FPS_TO_TPS(16.3);
double VirtualLRTEncoder::LOW_GEAR_MAX_RATE = FPS_TO_TPS(6.4);

VirtualLRTEncoder::VirtualLRTEncoder(UINT8 sourceA, UINT8 sourceB)
    : action(ActionData::GetInstance())
    , rate(0)
    , ticks(0)
{

}

VirtualLRTEncoder::~VirtualLRTEncoder()
{
    // nothing
}

INT32 VirtualLRTEncoder::Get()
{
    return ticks;
}

double VirtualLRTEncoder::GetRate()
{
    return rate;
}

// called at 50 Hz
void VirtualLRTEncoder::Update(int dutyCycle)
{
    switch(action.shifter.gear)
    {
    case LOW_GEAR:
        rate = LOW_GEAR_MAX_RATE;
        break;

    case HIGH_GEAR:
        rate = HIGH_GEAR_MAX_RATE;
        break;
    }

    // use dutyCycle to determine rate and add to the tick count
    rate *= dutyCycle;
    ticks += (int)(rate * 1.0 / 50.0);  // ticks / s * s; s = period = 1 / 50 Hz
}
