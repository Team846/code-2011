#include "AsynchronousPrinter.h"

#define PRINT 0

AsynchronousPrinter* AsynchronousPrinter::instance = NULL;

AsynchronousPrinter& AsynchronousPrinter::GetInstance()
{
    if(instance == NULL)
        instance = new AsynchronousPrinter();
    return *instance;
}

void AsynchronousPrinter::Printf(const char* format, ...)
{
#if !PRINT
    return;
#endif

    const int maxLength = 256;
    char buffer[maxLength];

    AsynchronousPrinter& me = GetInstance();

    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);
    va_end(args);

    {
        Synchronized s(me.semaphore);
        string str(buffer);

        me.queue.push(str);
        me.queueBytes += str.length();

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

AsynchronousPrinter::AsynchronousPrinter()
    : semaphore(semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE
            | SEM_INVERSION_SAFE))
    , queueBytes(0)
    , printerTask("AsynchronousPrinter",
            (FUNCPTR)AsynchronousPrinter::PrinterTaskRunner)
{
    AddToSingletonList();
    printerTask.Start();
}

AsynchronousPrinter::~AsynchronousPrinter()
{
    semDelete(semaphore);
}

void AsynchronousPrinter::PrinterTaskRunner()
{
    AsynchronousPrinter::GetInstance().PrinterTask();
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
                Synchronized s(semaphore);
                str = queue.front();

                queue.pop();
                queueBytes -= str.length();

                Wait(0.001);   // allow other tasks to run
            }

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
