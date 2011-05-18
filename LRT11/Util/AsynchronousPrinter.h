#ifndef LRT_ASYNCPRINT_H_
#define LRT_ASYNCPRINT_H_

#include "../General.h"
#include <queue>

class AsynchronousPrinter
{
public:
    virtual ~AsynchronousPrinter();
    static AsynchronousPrinter& Instance();

    static void Printf(const char* format, ...);
    void StopPrinterTask();
    void ResumePrinterTask();
    static void DeleteSingleton();
protected:
    AsynchronousPrinter();

private:
    static void PrinterTaskRunner();
    void PrinterTask();

    bool enabled;
    static AsynchronousPrinter* instance_;

    SEM_ID semaphore;
    int queueBytes;

    Task printerTask;
    queue<string> queue;

    const static int kMaxBuffer = 1024;
    DISALLOW_COPY_AND_ASSIGN(AsynchronousPrinter);

};

#endif
