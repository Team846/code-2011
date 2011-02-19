#include "EncoderDataCollection.h"

EncoderDataCollection::EncoderDataCollection()
    : Component()
    , leftEncoder(DriveEncoders::GetInstance().GetLeftEncoder())
    , left(RobotConfig::CAN_DRIVE_LEFT_A, leftEncoder, "left")
{

}

EncoderDataCollection::~EncoderDataCollection()
{

}

void EncoderDataCollection::Output()
{
    if(action.encoderData.shouldCollect)
    {
        for(int i = 1; i <= 4; i++)
        {
            left.Set(i * 0.25);
            ofstream fout(("EncoderData" + Util::ToString<int>(i)).c_str());

            for(int j = 0; j < 100; j++)
            {
                fout << leftEncoder.Get() << '\t' << leftEncoder.GetRate() << '\n';
                Wait(0.02); // 50 Hz; total of 2 seconds
            }

            fout.close();
        }
    }
}
