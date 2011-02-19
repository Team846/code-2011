#include "DBSDrive.h"

DBSDrive::DBSDrive(Esc& leftDrive, Esc& rightDrive, bool isSquaredInputs)
    : DriveMethod(leftDrive, rightDrive)
    , isSquaredInputs(isSquaredInputs)
    , prefix("DBSDrive.")
{
}

DBSDrive::~DBSDrive()
{
    // nothing to do
}

void DBSDrive::SetSquaredInputsEnabled(bool enabled)
{
    isSquaredInputs = enabled;
}

void DBSDrive::Configure()
{
    Config& config = Config::GetInstance();

    maxBraking = config.Get<int>(prefix + "maxBraking", 8);
    turnInPlaceThreshold = config.Get<float>(prefix + "turnInPlaceThreshold",
            10. / 128);
    brakeTurnDeadband = config.Get<float>(prefix + "brakeTurnDeadband",
            25. / 128);
}

DriveOutput DBSDrive::ComputeArcadeDrive(float forward, float turn)
{
    if(isSquaredInputs)
    {
        forward = Util::Sign<float>(forward) * forward * forward;
        turn = Util::Sign<float>(turn) * turn * turn;
    }

    enum
    {
        CW,
        CCW
    } turnDir;

    enum
    {
        LEFT,
        RIGHT
    } inboardSide;

    int brakeAmt;
    DriveOutput out;

    if(Util::Abs<float>(forward) < turnInPlaceThreshold)     // Normal turn-in-place
    {
        out.left = -turn;
        out.right = +turn;
        leftDrive.SetCoast();
        rightDrive.SetCoast();
    }
    else // Use DitherBraking
    {
        out.left = forward - turn;
        out.right = forward + turn;

        if(turn >= 0)
        {
            turnDir = CCW;
            inboardSide = (forward >= 0 ? LEFT : RIGHT);
        }
        else
        {
            turnDir = CW;
            inboardSide = (forward >= 0 ? RIGHT : LEFT);
        }

        // If we're turning within the deadband, then we only scale
        // down the power on one side. If we exceed the deadband,
        // then we apply successively greater braking to that side.

        float absTurn = Util::Abs<float>(turn);
        if(absTurn < brakeTurnDeadband)
        {
            float inboardSidePower = forward - (forward * absTurn / brakeTurnDeadband);
            if(inboardSide == LEFT)
                out.left = inboardSidePower;
            else
                out.right = inboardSidePower;
        }
        else // Use Dithered braking
        {

            brakeAmt = (int)(
                    (absTurn - brakeTurnDeadband)
                    * (maxBraking + 1) / (1 - brakeTurnDeadband)
                    ); // FIXME: make sure this int cast is okay.
            // brakeAmt on range {0, maxBraking}

            if(inboardSide == LEFT)
            {
                out.left = 0;
                leftDrive.ApplyBrakes(brakeAmt);
            }
            else
            {
                out.right = 0;
                rightDrive.ApplyBrakes(brakeAmt);
            }
        }
    }

    return out;
}
