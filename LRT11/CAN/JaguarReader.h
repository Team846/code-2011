#ifndef JAGUAR_READER_H_
#define JAGUAR_READER_H_

#include "..\General.h"
#include "..\Jaguar\ProxiedCANJaguar.h"
#include "../Util/PrintInConstructor.h"
class JaguarReader
{
private:
    PrintInConstructor print_ctor_dtor;
    static JaguarReader* instance;

    Task readerTask;
    semaphore* readSemaphore;

    JaguarReader();
    DISALLOW_COPY_AND_ASSIGN(JaguarReader);

public:
    static JaguarReader& GetInstance();
    ~JaguarReader();

    void StartReading();

    static void StartReaderTask();
    void ReaderTask();
    void StopTask();
};

#endif
