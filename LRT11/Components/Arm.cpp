#include "Arm.h"
#include "..\Config\RobotConfig.h"

Arm::Arm()
    : config(Config::GetInstance())
    , prefix("Arm.")
    , armEsc(RobotConfig::CAN::ARM_, "Arm")
#ifdef VIRTUAL
    // arm is ~29 inches
    // speed: 29 in * 1 ft / 12 in * 1.3 rps * 2 pi rad / rev = ~19.7 ft/s
    // ft / turn: 29 in * 1 ft / 12 in * 2 pi rad / rev = ~15.2 ft
    , armPot(RobotConfig::POT_ARM, 1, 15.2, 19.7)
#else
    , armPot(RobotConfig::ANALOG::POT_ARM)
#endif
    , state(IDLE)
    , oldState(action.arm.IDLE)
    , cycleCount(0)
    , presetMode(true)
    , pulseCount(0)
{
    // brake when set to 0 to keep the arm in place
    armEsc.ConfigNeutralMode(LRTCANJaguar::kNeutralMode_Brake);
    Configure();
    printf("Arm Constructed.\n");
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

    powerUp = config.Get<float>(prefix + "powerUp", 0.30);
    powerRetainUp = config.Get<float>(prefix + "powerRetainUp", 0.10);
    powerDown = config.Get<float>(prefix + "powerDown", -0.15);

    midPowerUp = config.Get<float>(prefix + "midPowerUp", 0.2);
    midPowerDown = config.Get<float>(prefix + "midPowerDown", -0.15);

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
        action.arm.doneState = action.arm.IN_PROGRESS;
        // overriden below to change roller speed while moving the arm up
        action.roller.maxSuckPower = 1.0;

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
            action.arm.doneState = action.arm.IN_PROGRESS;
            armEsc.Set(powerUp);

            action.roller.state = action.roller.SUCKING;
            action.roller.maxSuckPower = 0.3; // lower duty cycle

            // make roller suck while moving up to keep
            // game piece in
            if(++pulseCount % 2 == 0)
                action.roller.state = action.roller.SUCKING;
            else
                action.roller.state = action.roller.STOPPED;
        }
        break;

    case PRESET_BOTTOM:
        action.arm.doneState = action.arm.IN_PROGRESS;
        action.roller.maxSuckPower = 1.0;

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
            action.arm.doneState = action.arm.IN_PROGRESS;
            armEsc.Set(powerDown);
        }
        break;

    case PRESET_MIDDLE:
        action.arm.doneState = action.arm.IN_PROGRESS;

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
            armEsc.Set(midPowerDown);
        else if(potValue < midPosition - midPositionDeadband)
            armEsc.Set(midPowerUp);
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

        action.arm.doneState = action.arm.IN_PROGRESS;
        // operator must hold button to stay in manual mode
        action.arm.state = action.arm.IDLE;
        break;

    case MANUAL_DOWN:
        if(potValue > minPosition)
            armEsc.Set(powerDown);
        else
            armEsc.Set(0.0);

        action.arm.doneState = action.arm.IN_PROGRESS;
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
