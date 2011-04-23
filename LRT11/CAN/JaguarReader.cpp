#include "JaguarReader.h"

JaguarReader* JaguarReader::instance = NULL;

JaguarReader::JaguarReader()
    : readerTask("JaguarReaderTask", (FUNCPTR) StartReaderTask)
    , readSemaphore(semBCreate(SEM_Q_PRIORITY, SEM_EMPTY))
{
//    readerTask.Start();
}

JaguarReader& JaguarReader::GetInstance()
{
    if(instance == NULL)
        instance = new JaguarReader();
    return *instance;
}

// called by the main loop when there are free cycles
void JaguarReader::StartReading()
{
    semGive(readSemaphore);
}

void JaguarReader::StartReaderTask()
{
    JaguarReader::GetInstance().ReaderTask();
}

void JaguarReader::ReaderTask()
{
    // make large variable names more readable
#define jaguars ProxiedCANJaguar::jaguars
#define j jaguars.j

#define currents jaguars.currents
#define shouldCollectCurrent jaguars.shouldCollectCurrent

#define potValues jaguars.potValues
#define shouldCollectPotValue jaguars.shouldCollectPotValue

//    while(true)
//    {
    // wait until the end of the main loop with free cycles
//        semTake(readSemaphore, WAIT_FOREVER);

    for(int index = 0; index < jaguars.num; index++)
    {
        // collect current and pot value if necessary
        if(shouldCollectCurrent[index])
            currents[index] = j[index]->GetOutputCurrent();

        if(shouldCollectPotValue[index])
            potValues[index] = j[index]->GetPosition();
    }
//    }
}
