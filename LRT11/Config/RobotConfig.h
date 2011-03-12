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

#ifdef LRT_ROBOT_2011
    const static UINT32 CAN_DRIVE_LEFT_A  = 22;
    const static UINT32 CAN_DRIVE_LEFT_B  = 23;
    const static UINT32 CAN_DRIVE_RIGHT_A = 20;
    const static UINT32 CAN_DRIVE_RIGHT_B = 21;
#else
    const static UINT32 CAN_DRIVE_LEFT  = 2;
    const static UINT32 CAN_DRIVE_RIGHT = 3;
#endif

    const static UINT32 CAN_LIFT = 28;
    const static UINT32 CAN_ARM  = 24;

    const static UINT32 CAN_ROLLER_TOP    = 2;
    const static UINT32 CAN_ROLLER_BOTTOM = 3;

    // Servos

    // Relay Outputs

    // Digital Inputs
#ifdef LRT_ROBOT_2011
    const static UINT32 DIO_ENCODER_LEFT_A  = 9;
    const static UINT32 DIO_ENCODER_LEFT_B  = 10;
    const static UINT32 DIO_ENCODER_RIGHT_A = 13;
    const static UINT32 DIO_ENCODER_RIGHT_B = 14;
#else
    const static UINT32 DIO_ENCODER_LEFT_A  = 1;
    const static UINT32 DIO_ENCODER_LEFT_B  = 2;
    const static UINT32 DIO_ENCODER_RIGHT_A = 13;
    const static UINT32 DIO_ENCODER_RIGHT_B = 14;
#endif

    // Digital Outputs
    const static UINT32 DIO_BRAKE_LEFT  = CHANGEME;
    const static UINT32 DIO_BRAKE_RIGHT = CHANGEME;

    // Analog Inputs
    const static UINT32 POT_ARM = 3;

    // Servo Ports
#ifdef LRT_ROBOT_2011
    const static int LEFT_GEARBOX_SERVO_PORT  = 2;
    const static int RIGHT_GEARBOX_SERVO_PORT = 1;
#else
    const static int LEFT_GEARBOX_SERVO_PORT  = 8;
    const static int RIGHT_GEARBOX_SERVO_PORT = 10;
#endif
};

#endif
