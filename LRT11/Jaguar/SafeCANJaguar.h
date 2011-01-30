#ifndef SAFE_CAN_JAGUAR_H_
#define SAFE_CAN_JAGUAR_H_

#include "..\General.h"
#include "..\Util\AsynchronousPrinter.h"

/**
 * Luminary Micro Jaguar Speed Control
 */
class SafeCANJaguar : public CANJaguar
{
public:
    explicit SafeCANJaguar(UINT8 deviceNumber);
    virtual ~SafeCANJaguar();

    bool IsOnline();
    void CheckConnectivity();

    virtual void setTransaction(UINT32 messageID, const UINT8* data, UINT8 dataSize);
    virtual void getTransaction(UINT32 messageID, UINT8* data, UINT8* dataSize);

private:
    bool isEnabled;
};
#endif

