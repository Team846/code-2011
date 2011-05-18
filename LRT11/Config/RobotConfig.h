#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_

#include "../General.h"
#define CHANGEME 0

struct RobotConfig
{
#ifdef LRT_ROBOT_2011
    struct CAN
    {
        const static UINT32 DRIVE_RIGHT_B = 1;
        const static UINT32 DRIVE_LEFT_A  = 2;
        const static UINT32 DRIVE_LEFT_B  = 3;
        const static UINT32 ARM_          = 4;
        const static UINT32 ROLLER_BOTTOM = 5;
        const static UINT32 DEPLOYER      = 6;
        const static UINT32 ROLLER_TOP    = 7;
        const static UINT32 LIFT          = 8;
        const static UINT32 DRIVE_RIGHT_A = 10;
    } static CAN;

    struct PWM
    {
        const static UINT32 RIGHT_GEARBOX_SERVO = 1;
        const static UINT32 LEFT_GEARBOX_SERVO  = 2;
        const static UINT32 ALIGNER_SERVO       = 3;
    } static PWM_Port;

    // digital and analog IO
    const static UINT32 POT_ARM                  = 3;
    const static UINT32 LINE_SENSOR_SI           = 4;
    const static UINT32 LINE_SENSOR_CLOCK        = 5;
    const static UINT32 LINE_SENSOR_ADC          = 6;
    const static UINT32 DIO_ENCODER_LEFT_A       = 9;
    const static UINT32 DIO_ENCODER_LEFT_B       = 10;
    const static UINT32 DIO_ENCODER_RIGHT_A      = 13;
    const static UINT32 DIO_ENCODER_RIGHT_B      = 14;
#else
    struct CAN
    {
        const static UINT32 DRIVE_LEFT      = 2;
        const static UINT32 DRIVE_RIGHT     = 3;

        const static UINT32 ROLLER_TOP      = 32;
        const static UINT32 ROLLER_BOTTOM   = 22;

        // purposely invalid CAN IDs
//    const static UINT32 DRIVE_LEFT    = 0;
//    const static UINT32 DRIVE_RIGHT   = 0;
//    const static UINT32 ROLLER_TOP    = 0;
//    const static UINT32 CAN_ROLLER_BOTTOM = 0;
        const static UINT32 ARM_           = 0;
        const static UINT32 DEPLOYER      = 0;
        const static UINT32 LIFT          = 0;
    } static CAN;

    struct PWM
    {
        const static UINT32 LEFT_GEARBOX_SERVO  = 8;
        const static UINT32 RIGHT_GEARBOX_SERVO = 10;

        // invalid servos
        const static UINT32 ALIGNER_SERVO       = 0;
    } static PWM_Port;

    // digital and analog IO
    const static UINT32 DIO_ENCODER_LEFT_A  = 1;
    const static UINT32 DIO_ENCODER_LEFT_B  = 2;
    const static UINT32 DIO_ENCODER_RIGHT_A = 13;
    const static UINT32 DIO_ENCODER_RIGHT_B = 14;

    // purposely invalid digital and analog IO
    const static UINT32 POT_ARM             = 0;
    const static UINT32 LINE_SENSOR_SI      = 0;
    const static UINT32 LINE_SENSOR_CLOCK   = 0;
    const static UINT32 LINE_SENSOR_ADC     = 0;
#endif

};

#endif
