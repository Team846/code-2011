#include "VirtualPot.h"

VirtualPot::VirtualPot(int potTurns, float ftPerTurn, float motorMaxSpeedFps, float defaultPosition)
    : turns(potTurns)
    , distancePerTurn(ftPerTurn)
    , maxSpeed(motorMaxSpeedFps)
{
    // if default position is out of range, assume the half-way point
    position = defaultPosition;
    if(position < 0 || position > turns)
        position = turns / 2.0;
}

float VirtualPot::GetTurns()
{
    return turns;
}

// called at 50 Hz
void VirtualPot::Update(float dutyCycle)
{
    // use dutyCycle to determine rate and add to the turn count
    double rate = maxSpeed * dutyCycle;
    double distTraveled = rate * 1.0 / 50.0; // ticks / s * s = ticks; s = period = 1 / 50 Hz

    turns += (int)(distTraveled / distancePerTurn);
}
