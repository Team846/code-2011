#ifndef LRT_ASYNCLOGGER_H_
#define LRT_ASYNCLOGGER_H_

#include "../General.h"
#include <fstream>
#include <queue>
using namespace std;

class AsynchronousFileLogger : public SensorBase
{
public:
    virtual ~AsynchronousFileLogger();
    static AsynchronousFileLogger& GetInstance();

    static void Printf(const char* format, ...);
    void StopPrinterTask();
    void ResumePrinterTask();

protected:
	AsynchronousFileLogger();
    DISALLOW_COPY_AND_ASSIGN(AsynchronousFileLogger);

private:
    static void PrinterTaskRunner();
    void PrinterTask();

    bool enabled;
    static AsynchronousFileLogger* instance;

    SEM_ID semaphore;
    int queueBytes;

    Task printerTask;
    queue<string> queue;

    const static int kMaxBuffer = 1024;
    ofstream fout;
};

#endif
