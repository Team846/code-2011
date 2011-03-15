#include "MiniBotDeployer.h"
#include "..\Config\RobotConfig.h"

MiniBotDeployer::MiniBotDeployer()
    : config(Config::GetInstance())
    , runCount(0)
    , lastSetPoint(-0.00000564654)//garbage value
    , leftAlignerServo(RobotConfig::LEFT_ALIGNER_SERVO)
    , rightAlignerServo(RobotConfig::RIGHT_ALIGNER_SERVO)
    , deployerESC(RobotConfig::CAN_DEPLOYER)
{
    string prefix = "deployer.";
    left_locked = config.Get<float>(prefix + "left_locked_servo_val", 0.5);
    left_open = config.Get<float>(prefix + "left_open_servo_val", 0.5);
    deployedCurrent = config.Get<int>(prefix + "deployedCount", 20);
}

MiniBotDeployer::~MiniBotDeployer()
{

}


void MiniBotDeployer::Output()
{
    if(action.deployer.alignerReleased)
    {
        //assumes mirror image
        leftAlignerServo.Set(left_open);
        rightAlignerServo.Set(left_locked);
    }
    else
    {
        leftAlignerServo.Set(left_locked);
        rightAlignerServo.Set(left_open);
    }

    static enum
    {
        idle,
        accel,
        firstPush,
        relax,
        secondPush,
        deployed
    } state ;

    float setPoint;
    if(action.deployer.state == action.deployer.deployed)
    {
        switch(state)
        {
        case idle:
            state = accel;
            runCount = 0;
            break;
        case accel:
            setPoint = 1.0;
            if(++runCount > 10)
                state = firstPush;
            break;
        case firstPush:
            setPoint = 0.5;
            if(deployerESC.GetCurrent() > 15)
            {
                state = relax;
                runCount = 0;
            }
            break;
        case relax:
            setPoint = 0.0;
            if(++runCount < 25)  //0.5 seconds
            {
                state = secondPush;
                runCount = 0;
            }
            break;
        case secondPush:
            setPoint = 0.5;
            if(++runCount < 200)  // 4 seconds
                state = deployed;
            break;
        case deployed:
            //ne faire rien
            break;
        }
    }
    else if(action.deployer.state == action.deployer.retracted)
        setPoint = 0;
    else
        AsynchronousPrinter::Printf("Invalid deployer state\n");

    //do not send setpoint redundantly, especially if CANBUSController caching system is disabled
    if(setPoint != lastSetPoint)
        deployerESC.Set(setPoint);
    lastSetPoint = setPoint;
}
