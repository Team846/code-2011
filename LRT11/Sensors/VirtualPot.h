#ifndef VIRTUAL_POT_H_
#define VIRTUAL_POT_H_

#include "..\General.h"

class VirtualPot
{
private:
    int turns;
    float distancePerTurn; // ft
    float maxSpeed; // max speed of motor in ft / s, taking into account the gearbox
    float position;

public:
    VirtualPot(int potTurns, float distancePerTurn, float motorMaxSpeedFps, float defaultPosition = -1);
    ~VirtualPot();

    // in the range [0.0,turns]
    float GetTurns();

    // called at 50 Hz
    void Update(float dutyCycle);
};

#endif
