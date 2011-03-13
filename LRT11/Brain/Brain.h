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
#include "..\Sensors\LineSensor.h"

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

    // called from the main loop
    void Process(GameState gameState);

private:
    Config& config;
    Console& console;
    LCD& lcd;

    ActionData& action;
    GameState previousState;

    ProcessedInputs& inputs;
    Timer gameTimer;

#ifdef VIRTUAL
    VirtualLRTEncoder& leftEncoder;
    VirtualLRTEncoder& rightEncoder;
#else
    LRTEncoder& leftEncoder;
    LRTEncoder& rightEncoder;
#endif

    LineSensor lineSensor;
    int prevLinePosition;

    bool firstReading;
    bool leftSide;

    bool isFinale;

    // Mode-specific methods
    void Common();
    void Disabled();

    void Auton();
    void Teleop();

    // Subroutines used in multiple modes
    void AutomatedRoutines();

    // Teleop subroutines
    void TeleopDriveTrain();
    void TeleopShifter();

    void TeleopLift();
    void TeleopArm();

    void TeleopRoller();

    // SmartDashboard updating
    void UpdateDashboardValues(GameState gameState);
};

#endif
