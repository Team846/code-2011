#ifndef BRAIN_H_
#define BRAIN_H_

#include "..\General.h"
#include "..\ActionData.h"
#include "..\Util\Console.h"
#include "..\Util\LCD.h"
#include "..\Config\Config.h"
#include "..\DriverInputs\ProcessedInputs.h"
#include "..\Sensors\DriveEncoders.h"
#include "..\Sensors\LRTEncoder.h"

typedef enum GameState
{
    DISABLED,
    AUTONOMOUS,
    TELEOPERATED
};

class Brain
{
public:
    Brain();
    ~Brain();

    void Process(GameState gameState);

private:
    Config& config;
    Console& console;
    LCD& lcd;

    ActionData& action;
    GameState previousState;

    ProcessedInputs& inputs;
    Timer gameTimer;

    LRTEncoder& leftEncoder;
    LRTEncoder& rightEncoder;
    bool isFinale;

    void Common();
    void Disabled();

    void Auton();
    void Teleop();

    void TeleopDriveTrain();
    void TeleopShifter();

    void TeleopLift();
    void TeleopArm();

    void TeleopRoller();
    void UpdateDashboardValues(GameState gameState);
};

#endif
