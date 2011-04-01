#include "LiftPotReader.h"

LiftPotReader* LiftPotReader::instance = NULL;
ProxiedCANJaguar* LiftPotReader::liftEsc = NULL;

LiftPotReader::LiftPotReader()
    : readerTask("LiftPotReaderTask", (FUNCPTR) StartReaderTask)
    , liftPotVal(5) // default to the middle
{
    readSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
    readerTask.Start();
}

LiftPotReader::~LiftPotReader()
{
    // nothing
}

void LiftPotReader::SetLiftEsc(ProxiedCANJaguar* liftEsc)
{
    LiftPotReader::liftEsc = liftEsc;
}

LiftPotReader& LiftPotReader::GetInstance()
{
    if(instance == NULL)
        instance = new LiftPotReader();
    return *instance;
}

// starts the read
void LiftPotReader::StartReading()
{
    semGive(readSemaphore);
}

double LiftPotReader::GetPotValue()
{
    return liftPotVal;
}

void LiftPotReader::StartReaderTask()
{
    LiftPotReader::GetInstance().ReaderTask();
}

void LiftPotReader::ReaderTask()
{
    while(true)
    {
        semTake(readSemaphore, WAIT_FOREVER);
        if(liftEsc != NULL)
            liftPotVal = liftEsc->GetPosition();
        else
            AsynchronousPrinter::Printf("LiftEsc is NULL!");
    }
}
