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

    const static UINT32 CAN_DRIVE_LEFT_A  = 2;
    const static UINT32 CAN_DRIVE_LEFT_B  = 3;
    const static UINT32 CAN_DRIVE_RIGHT_A = 4;
    const static UINT32 CAN_DRIVE_RIGHT_B = 5;

    const static UINT32 CAN_LIFT = CHANGEME;
    const static UINT32 CAN_ARM  = CHANGEME;

    const static UINT32 CAN_ROLLER_TOP    = CHANGEME;
    const static UINT32 CAN_ROLLER_BOTTOM = CHANGEME;

    // Servos

    // Relay Outputs

    // Digital Inputs
    const static UINT32 DIO_ENCODER_LEFT_A  = 1;
    const static UINT32 DIO_ENCODER_LEFT_B  = 2;
    const static UINT32 DIO_ENCODER_RIGHT_A = 13;
    const static UINT32 DIO_ENCODER_RIGHT_B = 14;

    // Digital Outputs
    const static UINT32 DIO_BRAKE_LEFT  = CHANGEME;
    const static UINT32 DIO_BRAKE_RIGHT = CHANGEME;


    //Servo Ports
    const static int LEFT_GEARBOX_SERVO_PORT = 8;
    const static int RIGHT_GEARBOX_SERVO_PORT = 10;
};

#endif
