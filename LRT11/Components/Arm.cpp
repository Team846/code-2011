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
//   , state(IDLE)
    , oldState(ACTION::ARM_::IDLE)
    , cycleCount(0)
    , presetMode(true)
    , pulseCount(0)
{
    // brake when set to 0 to keep the arm in place
    armEsc.ConfigNeutralMode(LRTCANJaguar::kNeutralMode_Brake);
    Configure();
    printf("Constructed Arm\n");
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
        armEsc.SetDutyCycle(0.0);
        action.arm.state = ACTION::ARM_::IDLE;
        action.arm.completion_status = ACTION::ABORTED;
        return; // do not allow normal processing (ABORT is not printed...should fix this -dg)
    }

    const bool state_change = (oldState != action.arm.state);
    if(state_change)
        AsynchronousPrinter::Printf("Arm: %s\n", ACTION::ARM_::state_string[action.arm.state]);

    if(state_change)
        cycleCount = timeoutCycles; // reset timeout

    switch(action.arm.state)
    {
    case ACTION::ARM_::PRESET_TOP:
        action.arm.completion_status = ACTION::IN_PROGRESS;
        // overriden below to change roller speed while moving the arm up
        action.roller.maxSuckPower = 1.0;

        // don't merely switch to the IDLE state, as the caller will likely
        // set the state each time through the loop
        if(--cycleCount < 0)
        {
            action.arm.completion_status = ACTION::FAILURE;
            armEsc.SetDutyCycle(0.0);
            break; // timeout overrides everything
        }

        if(potValue >= maxPosition)
        {
            action.arm.completion_status = ACTION::SUCCESS;
            armEsc.SetDutyCycle(powerRetainUp);
            // cycleCount will never get decremented below 0, so powerRetainUp
            // will be maintained
            cycleCount = 100;
        }
        else
        {
            action.arm.completion_status = ACTION::IN_PROGRESS;
            armEsc.SetDutyCycle(powerUp);

            action.roller.state = ACTION::ROLLER::SUCKING;
            action.roller.maxSuckPower = 0.3; // lower duty cycle

            // make roller suck while moving up to keep
            // game piece in
            if(++pulseCount % 2 == 0)
                action.roller.state = ACTION::ROLLER::SUCKING;
            else
                action.roller.state = ACTION::ROLLER::STOPPED;
        }
        break;

    case ACTION::ARM_::PRESET_BOTTOM:
        action.arm.completion_status = ACTION::IN_PROGRESS;
        action.roller.maxSuckPower = 1.0;

        if(--cycleCount < 0)
        {
            action.arm.completion_status = ACTION::FAILURE;
            armEsc.SetDutyCycle(0.0);
            break; // timeout overrides everything
        }

        if(potValue <= minPosition)
        {
            action.arm.completion_status = ACTION::SUCCESS;
            armEsc.SetDutyCycle(0.0); // don't go below the min position
            // cycleCount will never get decremented below 0, so powerRetainUp
            // will be maintained
            cycleCount = 100;
        }
        else
        {
            action.arm.completion_status = ACTION::IN_PROGRESS;
            armEsc.SetDutyCycle(powerDown);
        }
        break;

    case ACTION::ARM_::PRESET_MIDDLE:
        action.arm.completion_status = ACTION::IN_PROGRESS;

        // no timeout for now
//      if(--cycleCount < 0)
//      {
//          action.arm.status = ACTION::FAILURE;
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
            armEsc.SetDutyCycle(midPowerDown);
        else if(potValue < midPosition - midPositionDeadband)
            armEsc.SetDutyCycle(midPowerUp);
        else
        {
            // prevent cycle count from becoming < 0
            cycleCount = 100;
            action.arm.completion_status = ACTION::SUCCESS;
            armEsc.SetDutyCycle(0.0);
        }
        break;

    case ACTION::ARM_::MANUAL_UP:
        if(potValue < maxPosition)
            armEsc.SetDutyCycle(powerUp);
        else
            armEsc.SetDutyCycle(0.0);

        action.arm.completion_status = ACTION::IN_PROGRESS;
        // operator must hold button to stay in manual mode
        action.arm.state = ACTION::ARM_::IDLE;
        break;

    case ACTION::ARM_::MANUAL_DOWN:
        if(potValue > minPosition)
            armEsc.SetDutyCycle(powerDown);
        else
            armEsc.SetDutyCycle(0.0);

        action.arm.completion_status = ACTION::IN_PROGRESS;
        // operator must hold button to stay in manual mode
        action.arm.state = ACTION::ARM_::IDLE;
        break;

    case ACTION::ARM_::IDLE:
        action.arm.completion_status = ACTION::SUCCESS;
        armEsc.SetDutyCycle(0.0);
        break;
    default:
        AsynchronousPrinter::Printf("Arm: ERROR Unknown State\n");

    }

    oldState = action.arm.state;

    //Print diagnostics
    static ACTION::eCompletionStatus lastDoneState = ACTION::UNSET;
    if(lastDoneState != action.arm.completion_status)
        AsynchronousPrinter::Printf("Arm: Status=%s\n", ACTION::status_string[action.arm.completion_status]);
    lastDoneState = action.arm.completion_status;
}
