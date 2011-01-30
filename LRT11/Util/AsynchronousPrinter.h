#ifndef LRT_ASYNCPRINT_H_
#define LRT_ASYNCPRINT_H_

#include "../General.h"
#include <queue>
using namespace std;

class AsynchronousPrinter : public SensorBase
{
public:
    virtual ~AsynchronousPrinter();
    static AsynchronousPrinter& GetInstance();

    static void Printf(const char* format, ...);
    void StopPrinterTask();
    void ResumePrinterTask();

protected:
    AsynchronousPrinter();
    DISALLOW_COPY_AND_ASSIGN(AsynchronousPrinter)
    ;

private:
    static void PrinterTaskRunner();
    void PrinterTask();

    bool enabled;
    static AsynchronousPrinter* instance;

    Task printerTask;
    SEM_ID semaphore;

    queue<string> queue;
    int queueBytes;

    const static int kMaxBuffer = 1024;
};

#endif
