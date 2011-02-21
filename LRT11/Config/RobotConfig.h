#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_

#include "../General.h"
#define CHANGEME 0

class RobotConfig
{
public:
    // Jaguars
    const static UINT32 PWM_DRIVE_LEFT  = CHANGEME;
    const static UINT32 PWM_DRIVE_RIGHT = CHANGEME;

    const static UINT32 CAN_DRIVE_LEFT_A  = 22;
    const static UINT32 CAN_DRIVE_LEFT_B  = 23;
    const static UINT32 CAN_DRIVE_RIGHT_A = 20;
    const static UINT32 CAN_DRIVE_RIGHT_B = 21;

    const static UINT32 CAN_LIFT = 28;
    const static UINT32 CAN_ARM  = CHANGEME;

    const static UINT32 CAN_ROLLER_TOP    = CHANGEME;
    const static UINT32 CAN_ROLLER_BOTTOM = CHANGEME;

    // Servos

    // Relay Outputs

    // Digital Inputs
    const static UINT32 DIO_ENCODER_LEFT_A  = 9;
    const static UINT32 DIO_ENCODER_LEFT_B  = 10;
    const static UINT32 DIO_ENCODER_RIGHT_A = 13;
    const static UINT32 DIO_ENCODER_RIGHT_B = 14;

    // Digital Outputs
    const static UINT32 DIO_BRAKE_LEFT  = CHANGEME;
    const static UINT32 DIO_BRAKE_RIGHT = CHANGEME;


    //Servo Ports
    const static int LEFT_GEARBOX_SERVO_PORT  = CHANGEME;
    const static int RIGHT_GEARBOX_SERVO_PORT = CHANGEME;
};

#endif
