#include "AsynchronousPrinter.h"

#define PRINT 1

AsynchronousPrinter* AsynchronousPrinter::instance_ = NULL;

AsynchronousPrinter& AsynchronousPrinter::Instance()
{
    if(instance_ == NULL)
        instance_ = new AsynchronousPrinter();
    return *instance_;
}

void AsynchronousPrinter::Printf(const char* format, ...)
{
#if !PRINT
    return;
#endif
    char buffer[256];

    AsynchronousPrinter& me = Instance();

    va_list args;
    va_start(args, format);

    int n_bytes =  vsprintf(buffer, format, args);
    va_end(args);

    if(n_bytes >= 0)
    {
        Synchronized s(me.semaphore);
        string str(buffer);

        me.queue.push(str);
        me.queueBytes += n_bytes;

        if(me.queueBytes >= kMaxBuffer)
        {
            while(!me.queue.empty())
                me.queue.pop();

            string overflow("(AsyncPrinter Buffer Overflow)\n");

            me.queue.push(overflow);
            me.queueBytes = overflow.length();
        }
    }
}

AsynchronousPrinter::AsynchronousPrinter() :
    semaphore(semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE)),
    queueBytes(0),
    printerTask("AsynchronousPrinter", (FUNCPTR)AsynchronousPrinter::PrinterTaskRunner)
{
//    AddToSingletonList();
    printerTask.Start();
}

AsynchronousPrinter::~AsynchronousPrinter()
{
    semDelete(semaphore);
}

void AsynchronousPrinter::DeleteSingleton()
{
    if(NULL == instance_)
        return;
    delete instance_;
    instance_ = NULL;
}

void AsynchronousPrinter::PrinterTaskRunner()
{
    Instance().PrinterTask();
}
void AsynchronousPrinter::PrinterTask()
{
#if !PRINT
    return;
#endif

    while(true)
    {
        while(!queue.empty())
        {
            string str;

            {
                //Critical block
                Synchronized s(semaphore);
                str = queue.front();
                queue.pop();
                queueBytes -= str.length();
            }

            Wait(0.001);   // allow other tasks to run
            printf(str.c_str());
        }

        Wait(0.002);   // allow other tasks to run
    }
}

void AsynchronousPrinter::StopPrinterTask()
{
    printerTask.Suspend();
}

void AsynchronousPrinter::ResumePrinterTask()
{
    printerTask.Resume();
}
