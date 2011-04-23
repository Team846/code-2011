#ifndef JAGUAR_READER_H_
#define JAGUAR_READER_H_

#include "..\General.h"
#include "..\Jaguar\ProxiedCANJaguar.h"

class JaguarReader
{
private:
    static JaguarReader* instance;

    Task readerTask;
    SEM_ID readSemaphore;

    JaguarReader();
    DISALLOW_COPY_AND_ASSIGN(JaguarReader);

public:
    static JaguarReader& GetInstance();
    ~JaguarReader();

    void StartReading();

    static void StartReaderTask();
    void ReaderTask();
};

#endif
