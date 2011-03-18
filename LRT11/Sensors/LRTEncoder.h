
#ifndef LRT_ENCODER_H_
#define LRT_ENCODER_H_

#include "..\General.h"

/*
 * LRTEncoder patches bugs in WPILib's Encoder class.
 */
class LRTEncoder : public Encoder
{
private:
//  static int count = 0;
//    Encoder useless;

public:
    LRTEncoder(UINT8 sourceA, UINT8 sourceB);
    ~LRTEncoder();

    double GetRate();
};

#endif
