#ifndef LIFT_POT_READER_H_
#define LIFT_POT_READER_H_

#include "..\..\General.h"
#include "..\..\Jaguar\ProxiedCANJaguar.h"
#include <semLib.h>

class LiftPotReader
{
private:
    static LiftPotReader* instance;
    static ProxiedCANJaguar* liftEsc;

    SEM_ID readSemaphore;
    Task readerTask;
    double liftPotVal;

    LiftPotReader();
    DISALLOW_COPY_AND_ASSIGN(LiftPotReader);

public:
    ~LiftPotReader();

    static void SetLiftEsc(ProxiedCANJaguar* liftEsc);
    static LiftPotReader& GetInstance();

    void StartReading();
    double GetPotValue();

    static void StartReaderTask();
    void ReaderTask();
};

#endif
