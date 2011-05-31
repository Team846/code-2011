#include "JaguarReader.h"

//Obsolete -- all references to JaguarReader in the program should be removed. -dg 5/30/2011 TODO
JaguarReader* JaguarReader::instance = NULL;

JaguarReader::JaguarReader()
    : print_ctor_dtor("JaguarReader\n")
    , readerTask("JaguarReaderTask", (FUNCPTR) StartReaderTask)
    , readSemaphore(semBCreate(SEM_Q_PRIORITY, SEM_EMPTY))
{
    readerTask.Start();
}

JaguarReader& JaguarReader::GetInstance()
{
    printf("JAGUAR READER: Get Instance\n");    //shouldn't be here.
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
void JaguarReader::StopTask()
{
    JaguarReader::GetInstance().readerTask.Stop();
}


void JaguarReader::ReaderTask()
{
#if 0 //CUT
// Jaguar tasks may have died or the jag objects may have been destroyed.
// This task must be robust to check. -dg

    // make large variable names more readable
#define jaguars ProxiedCANJaguar::jaguars
#define j jaguars.j

#define currents jaguars.currents
#define shouldCollectCurrent jaguars.shouldCollectCurrent

#define potValues jaguars.potValues
#define shouldCollectPotValue jaguars.shouldCollectPotValue

    while(true)
    {
        // wait until the end of the main loop with free cycles
        semTake(readSemaphore, WAIT_FOREVER);

        for(int index = 0; index < jaguars.num; index++)
        {
            // collect current and pot value if necessary
            if(shouldCollectCurrent[index])
                currents[index] = j[index]->GetOutputCurrent();

            if(shouldCollectPotValue[index])
                potValues[index] = j[index]->GetPosition();
        }
    }
#endif // CUT
}
