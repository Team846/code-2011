#ifndef ENCODER_DATA_COLLECTION
#define ENCODER_DATA_COLLECTION

#include "Component.h"
#include "..\Sensors\DriveEncoders.h"
#include "..\Jaguar\Esc.h"
#include "..\Config\RobotConfig.h"
#include "..\Util\Util.h"
#include <fstream>
using namespace std;

class EncoderDataCollection : public Component
{
private:
	LRTEncoder& leftEncoder;
	Esc left;

public:
    EncoderDataCollection();
    virtual ~EncoderDataCollection();

    virtual void Output();
};

#endif
