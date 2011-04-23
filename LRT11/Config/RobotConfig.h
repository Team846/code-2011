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
    const static UINT32 CAN_DRIVE_RIGHT_A = 10;
    const static UINT32 CAN_DRIVE_RIGHT_B = 1;
    const static UINT32 CAN_DRIVE_LEFT_A  = 2;
    const static UINT32 CAN_DRIVE_LEFT_B  = 3;
#else
//    const static UINT32 CAN_DRIVE_LEFT  = 2;
//    const static UINT32 CAN_DRIVE_RIGHT = 3;
    const static UINT32 CAN_DRIVE_LEFT  = 22;
    const static UINT32 CAN_DRIVE_RIGHT = 32;
#endif

    const static UINT32 CAN_LIFT = 8;
    const static UINT32 CAN_ARM  = 4;

#ifdef LRT_ROBOT_2011
    const static UINT32 CAN_ROLLER_TOP    = 7;
    const static UINT32 CAN_ROLLER_BOTTOM = 5;
#else
    const static UINT32 CAN_ROLLER_TOP    = 3;
    const static UINT32 CAN_ROLLER_BOTTOM = 2;
#endif

    const static UINT32 CAN_DEPLOYER = 6;

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
    const static UINT32 LINE_SENSOR_CLOCK = 5;
    const static UINT32 LINE_SENSOR_SI = 4;

    // Analog Inputs
    const static UINT32 POT_ARM = 3;
    const static UINT32 LINE_SENSOR_ADC = 6;

    // Servo Ports
#ifdef LRT_ROBOT_2011
    const static UINT32 LEFT_GEARBOX_SERVO_PORT  = 2;
    const static UINT32 RIGHT_GEARBOX_SERVO_PORT = 1;
#else
    const static UINT32 LEFT_GEARBOX_SERVO_PORT  = 8;
    const static UINT32 RIGHT_GEARBOX_SERVO_PORT = 10;
#endif

    const static UINT32 ALIGNER_SERVO = 3;
};

#endif
