#include "SafeCANJaguar.h"

SafeCANJaguar::SafeCANJaguar(UINT8 deviceNumber) :
	CANJaguar(deviceNumber)
	, isEnabled(true)
{
	CheckConnectivity();
}

SafeCANJaguar::~SafeCANJaguar()
{
	// nothing
}

bool SafeCANJaguar::IsOnline()
{
	float volts = GetBusVoltage();
	AsynchronousPrinter::Printf("CAN %d : %f\n", m_deviceNumber, volts);
	return volts > 2.0;
}

void SafeCANJaguar::CheckConnectivity()
{
	isEnabled = true;
	isEnabled = IsOnline();
}

void SafeCANJaguar::setTransaction(UINT32 messageID, const UINT8* data, UINT8 dataSize)
{
	if(isEnabled)
		CANJaguar::setTransaction(messageID, data, dataSize);
	else
	{
		// ne faire rien...
	}
}

void SafeCANJaguar::getTransaction(UINT32 messageID, UINT8* data, UINT8* dataSize)
{
	if(isEnabled)
		CANJaguar::getTransaction(messageID, data, dataSize);
	else
	{
		// ne faire rien...
	}
}
