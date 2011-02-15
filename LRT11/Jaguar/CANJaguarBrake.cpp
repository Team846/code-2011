#include "CANJaguarBrake.h"

// Note: Should this also contain the SpeedController object?
// And force it to 0 output when brakelevel > 0
CANJaguarBrake::CANJaguarBrake(ProxiedCANJaguar& jaggie)
    : jaguar(jaggie), cycleCount(0), amount(0)
{
}

void CANJaguarBrake::ApplyBrakes(int brakeAmount)
{
    AsynchronousPrinter::Printf("Apply brakes\n");
    amount = Util::Clamp<int>(brakeAmount, 0, 8);
}

void CANJaguarBrake::UpdateOutput()
{
    // 1-byte bitfields corresponding to value. See below for calculation.
    static const UINT8 ditherPattern[] = {0x00, 0x01, 0x11, 0x25, 0x55, 0xD5, 0xEE, 0xFE, 0xFF};

    ++cycleCount;
    if(cycleCount >= 8)
        cycleCount = 0;

    AsynchronousPrinter::Printf("Updating output\n");
    if(ditherPattern[amount] & (1 << cycleCount))
        jaguar.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
    else
        jaguar.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
}

/*
 * Easier to understand way:
 * Each integer, corresponding to value, is a bitfield of 8 cycles
 * Pattern N has N bits out of 8 set to true.
 * 0: 0000 0000 = 0x00
 * 1: 0000 0001 = 0x01
 * 2: 0001 0001 = 0x11
 * 3: 0010 0101 = 0x25
 * 4: 0101 0101 = 0x55
 * 5: 1101 0101 = 0xD5
 * 6: 1110 1110 = 0xEE
 * 7: 1111 1110 = 0xFE
 * 8: 1111 1111 = 0xFF
 *
 * Old way:
 * 001
 * 010
 * 011
 * 100
 * 101
 * 110
 * 111
 */
