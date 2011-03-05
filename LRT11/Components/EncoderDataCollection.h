#ifndef ENCODER_DATA_COLLECTION_H_
#define ENCODER_DATA_COLLECTION_H_

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
    VirtualLRTEncoder& leftEncoder;
    Esc left;

public:
    EncoderDataCollection();
    virtual ~EncoderDataCollection();

    virtual void Output();
};

#endif
