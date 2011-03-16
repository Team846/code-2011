#include "MiniBotDeployer.h"
#include "..\Config\RobotConfig.h"

MinibotDeployer::MinibotDeployer()
    : config(Config::GetInstance())
    , deployerEsc(RobotConfig::CAN_DEPLOYER)
    , leftAlignerServo(RobotConfig::LEFT_ALIGNER_SERVO)
    , rightAlignerServo(RobotConfig::RIGHT_ALIGNER_SERVO)
{
    string prefix = "MinibotDeployer.";
    lockedServoValue = config.Get<float>(prefix + "lockedServoValue", 1.0);
    releasedServoValue = config.Get<float>(prefix + "releasedServoValue", 0.0);
    currentThreshold = config.Get<int>(prefix + "currentThreshold", 15);
}

MinibotDeployer::~MinibotDeployer()
{

}

void MinibotDeployer::Output()
{
    if(action.deployer.shouldAlignerRelease)
    {
        // set both servos to release position
        leftAlignerServo.Set(releasedServoValue);
        rightAlignerServo.Set(releasedServoValue);
    }
    else
    {
        // set both servos to locked position
        leftAlignerServo.Set(lockedServoValue);
        rightAlignerServo.Set(lockedServoValue);
    }

    static enum
    {
        IDLE,
        ACCELERATING,
        FIRST_PUSH,
        RELAX,
        SECOND_PUSH
    } state = IDLE;

    static int timer = 0;
    if(state == IDLE && action.deployer.shouldDeployMinibot)
    {
        timer = 0; // reset timer
        state = ACCELERATING;

        // deployment already commenced
        action.deployer.shouldDeployMinibot = false;
    }

    // used to detect current spike
    float current = deployerEsc.GetCurrent();
    SmartDashboard::Log(current, "Minibot Deployment Current");

    float setPoint;
    switch(state)
    {
    case IDLE:
        // no movement
        setPoint = 0;
        break;

    case ACCELERATING:
        // full power
        setPoint = 1.0;

        // two fifths second pulse of full power
        if(++timer % 20 == 0)
        {
            state = FIRST_PUSH;
            timer = 0;
        }

        break;

    case FIRST_PUSH:
        // full power
        setPoint = 0.5;

        // wait until a current spike or 1 second timeout
        if(current > currentThreshold || ++timer % 50 == 0)
        {
            state = RELAX; // relax for a bit
            timer = 0;
        }

        break;

    case RELAX:
        // no power
        setPoint = 0.0;

        // half a second of relaxing
        if(++timer % 25 == 0)
        {
            state = SECOND_PUSH; // next push to actually deploy the minibot
            timer = 0;
        }

        break;

    case SECOND_PUSH:
        // second push requires less power
        setPoint = 0.5;

        // one second of pushing
        if(++timer % 50 == 0)
        {
            state = IDLE; // done!
            timer = 0;
        }

        break;
    }

    deployerEsc.Set(setPoint);
}
