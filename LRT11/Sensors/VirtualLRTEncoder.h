#ifndef VIRTUAL_LRT_ENCODER_H_
#define VIRTUAL_LRT_ENCODER_H_

#include "..\General.h"
#include "..\ActionData.h"

class VirtualLRTEncoder
{
private:
    // used to determine if low or high gear
    ActionData& action;

    double rate; // ticks/sec
    int ticks;

    // conversion from ft / s rate input to ticks / s
    // ft / s * in / ft * rev / in * ticks / rev = ticks / s
    // in ticks / s
    static double HIGH_GEAR_MAX_RATE;
    static double LOW_GEAR_MAX_RATE;

    enum {LOW_GEAR = 1 , HIGH_GEAR = 2};

public:
    VirtualLRTEncoder(UINT8 sourceA, UINT8 sourceB);
    ~VirtualLRTEncoder();

    void SetDistancePerPulse(double distancePerPulse) {}
    void Start() {}

    INT32 Get();
    double GetRate();

    // called at 50 Hz
    void Update(int dutyCycle);
};

#endif
