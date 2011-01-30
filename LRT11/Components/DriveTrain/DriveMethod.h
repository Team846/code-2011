//Author: David Liu (2009)

#ifndef DRIVEMETHOD_H
#define DRIVEMETHOD_H

#include "..\..\General.h"
#include "..\..\Util\Util.h"

// Warning: You MUST override ComputeTankDrive or ComputeArcadeDrive
// or an infinite loop will occur.
struct DriveOutput
{
    float left, right;

    DriveOutput() :
        left(0)
        , right(0) {}

    DriveOutput(float leftDrive, float rightDrive) :
        left(leftDrive)
        , right(rightDrive) {}

    bool operator ==(DriveOutput other)
    {
        return other.left == left && other.right == right;
    }
};

const static DriveOutput NO_OUTPUT = DriveOutput(100, 100);

class DriveMethod
{
protected:
    Esc& leftDrive, &rightDrive;
    DriveMethod(Esc& leftDrive, Esc& rightDrive)
        : leftDrive(leftDrive), rightDrive(rightDrive)
    {
        // Must be subclassed!
    }

    virtual DriveOutput ComputeTankDrive(float left, float right)
    {
        float forward = (left + right) / 2;
        float turn = (right - left) / 2;
        return ComputeArcadeDrive(forward, turn);
    }
    virtual DriveOutput ComputeArcadeDrive(float forward, float turn)
    {
        float left = forward - turn;
        float right = forward + turn;
        return ComputeTankDrive(left, right);
    }

    virtual void SetDriveOutputs(DriveOutput speeds)
    {
        if(speeds == NO_OUTPUT)
            return;

        // speeds are clamped in LRTEsc
        leftDrive.Set(speeds.left);
        rightDrive.Set(speeds.right);
    }

public:

    void TankDrive(float left, float right)
    {
        left = Util::Clamp<float>(left, -1, 1);
        right = Util::Clamp<float>(right, -1, 1);
        SetDriveOutputs(ComputeTankDrive(left, right));
    }

    void ArcadeDrive(float forward, float turn)
    {
        forward = Util::Clamp<float>(forward, -1, 1);
        turn = Util::Clamp<float>(turn, -1, 1);
        SetDriveOutputs(ComputeArcadeDrive(forward, turn));
    }

};

#endif
