#include "Arm.h"
#include "..\Config\RobotConfig.h"

Arm::Arm()
    : config(Config::GetInstance())
    , prefix("Arm.")
    , armEsc(RobotConfig::CAN_ARM)
#ifdef VIRTUAL
    // arm is ~29 inches
    // speed: 29 in * 1 ft / 12 in * 1.3 rps * 2 pi rad / rev = ~19.7 ft/s
    // ft / turn: 29 in * 1 ft / 12 in * 2 pi rad / rev = ~15.2 ft
    , armPot(RobotConfig::POT_ARM, 1, 15.2, 19.7)
#else
    , armPot(RobotConfig::POT_ARM)
#endif
    , state(IDLE)
    , oldState(action.arm.IDLE)
    , cycleCount(0)
    , presetMode(true)
    , pulseCount(0)
{
    // brake when set to 0 to keep the arm in place
    armEsc.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
    Configure();
}

Arm::~Arm()
{

}

void Arm::Configure()
{
    minPosition = config.Get<float>(prefix + "minPosition", 280);
    midPosition = config.Get<float>(prefix + "midPosition", 621);
    maxPosition = config.Get<float>(prefix + "maxPosition", 530);

    midPositionDeadband = config.Get<float>(prefix + "midPositionDeadband", 10);
    midPGain = config.Get<float>(prefix + "MidPGain");

    powerUp = config.Get<float>(prefix + "powerUp", 0.30);
    powerRetainUp = config.Get<float>(prefix + "powerRetainUp", 0.10);
    powerDown = config.Get<float>(prefix + "powerDown", -0.15);

    timeoutCycles = (int)(config.Get<int>(prefix + "timeoutMs", 1500) * 1.0 / 1000.0 * 50.0 / 1.0);
}

void Arm::Output()
{
    float potValue = armPot.GetAverageValue();

#ifdef USE_DASHBOARD
    SmartDashboard::Log(potValue, "Arm Pot Value");
#endif

    if(action.master.abort)
    {
        armEsc.Set(0.0);
        action.arm.state = action.arm.IDLE;
        action.arm.doneState = action.arm.ABORTED;
        return; // do not allow normal processing
    }

    if(oldState != action.arm.state)
        cycleCount = timeoutCycles; // reset timeout

    switch(action.arm.state)
    {
    case PRESET_TOP:
        action.arm.doneState = action.arm.STALE;

        // don't merely switch to the IDLE state, as the caller will likely
        // set the state each time through the loop
        if(--cycleCount < 0)
        {
            action.arm.doneState = action.arm.FAILURE;
            armEsc.Set(0.0);
            break; // timeout overrides everything
        }

        if(potValue >= maxPosition)
        {
            action.arm.doneState = action.arm.SUCCESS;
            armEsc.Set(powerRetainUp);
            // cycleCount will never get decremented below 0, so powerRetainUp
            // will be maintained
            cycleCount = 100;
        }
        else
        {
            action.arm.doneState = action.arm.STALE;
            armEsc.Set(powerUp);

            // make roller suck while moving up to keep
            // game piece in
            if(++pulseCount % 2 == 0)
                action.roller.state = action.roller.SUCKING;
            else
                action.roller.state = action.roller.STOPPED;
        }
        break;

    case PRESET_BOTTOM:
        action.arm.doneState = action.arm.STALE;
        if(--cycleCount < 0)
        {
            action.arm.doneState = action.arm.FAILURE;
            armEsc.Set(0.0);
            break; // timeout overrides everything
        }

        if(potValue <= minPosition)
        {
            action.arm.doneState = action.arm.SUCCESS;
            armEsc.Set(0.0); // don't go below the min position
            // cycleCount will never get decremented below 0, so powerRetainUp
            // will be maintained
            cycleCount = 100;
        }
        else
        {
            action.arm.doneState = action.arm.STALE;
            armEsc.Set(powerDown);
        }
        break;

    case PRESET_MIDDLE:
        action.arm.doneState = action.arm.STALE;

        // no timeout for now
//      if(--cycleCount < 0)
//      {
//          action.arm.doneState = action.arm.FAILURE;
//          armEsc.Set(0.0);
//          break; // timeout overrides everything
//      }
//      float error = potValue - midPosition;
//      float correction = error * midPGain;
//      if (Util::Abs<float>(error) < midPositionDeadband)
//          armEsc.Set(0.0);
//      else
//          armEsc.Set(correction);

        if(potValue > midPosition + midPositionDeadband)
            armEsc.Set(powerDown / 2);
        else if(potValue < midPosition - midPositionDeadband)
            armEsc.Set(powerUp / 2);
        else
        {
            // prevent cycle count from becoming < 0
            cycleCount = 100;
            action.arm.doneState = action.arm.SUCCESS;
            armEsc.Set(0.0);
        }
        break;

    case MANUAL_UP:
        if(potValue < maxPosition)
            armEsc.Set(powerUp);
        else
            armEsc.Set(0.0);

        action.arm.doneState = action.arm.STALE;
        // operator must hold button to stay in manual mode
        action.arm.state = action.arm.IDLE;
        break;

    case MANUAL_DOWN:
        if(potValue > minPosition)
            armEsc.Set(powerDown);
        else
            armEsc.Set(0.0);

        action.arm.doneState = action.arm.STALE;
        // operator must hold button to stay in manual mode
        action.arm.state = action.arm.IDLE;
        break;

    case IDLE:
        action.arm.doneState = action.arm.SUCCESS;
        armEsc.Set(0.0);
        break;
    }

    oldState = action.arm.state;
}
