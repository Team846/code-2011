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

    const static UINT32 CAN_DRIVE_LEFT  = 2;
    const static UINT32 CAN_DRIVE_RIGHT = 3;

    const static UINT32 CAN_LIFT = 4;
    const static UINT32 CAN_ARM = 4;

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
