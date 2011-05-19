#include "AsynchronousPrinter.h"

#define PRINT 1


AsynchronousPrinter& AsynchronousPrinter::Instance()
{
    static AsynchronousPrinter printer;
    return printer;
}

AsynchronousPrinter::AsynchronousPrinter()
    : quitting_(false)
    , running_(false)
    , semaphore(semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE))
    , queueBytes(0)
    , printerTask("AsynchronousPrinter", (FUNCPTR)AsynchronousPrinter::PrinterTaskRunner)
{
    printerTask.Start();
}

AsynchronousPrinter::~AsynchronousPrinter()
{
    Quit(); //kill the running print tasks.
    if(running_)
        Wait(0.010); //wait for print tasks to die. (will they die? -dg)

    semDelete(semaphore);
}

int AsynchronousPrinter::Printf(const char* format, ...)
{
#if !PRINT
    return 0; // # of bytes printed
#endif
    char buffer[256];

    AsynchronousPrinter& me = Instance();
    if(me.quitting_)   //the program is quitting. Abort.
        return 0; // # of bytes printed

    va_list args;
    va_start(args, format);

    int n_bytes =  vsprintf(buffer, format, args);
    va_end(args);

    if(n_bytes >= 0)
    {
        Synchronized s(me.semaphore);
        if(me.quitting_)   //the program is quitting & waiting for us.  Stop.
            return 0; // # of bytes printed.

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
    return n_bytes;
}




//May be called externally to stop printing.
void AsynchronousPrinter::Quit()
{
    Instance().quitting_ = true;
}

int AsynchronousPrinter::PrinterTaskRunner()
{
#if !PRINT
    Instance().running_ = false;
    return 0; //printer task dies.
#endif
    Instance().running_ = true;
    int status = Instance().PrinterTask();
    Instance().running_ = false;
    printf("Stopping Async Printing\n");
    return status;
}

int AsynchronousPrinter::PrinterTask()
{
    while(!quitting_)
    {
        while(!quitting_ && !queue.empty())
        {
            string str;
            {
                //Critical block
                Synchronized s(semaphore);
                if(quitting_)   //the program is quitting & waiting for us.  Stop.
                    return 0;
                str = queue.front();
                queue.pop();
                queueBytes -= str.length();
            }

            printf(str.c_str());
            Wait(0.001);   // allow other tasks to run
        }

        Wait(0.002);   // allow other tasks to run
    }
    return 0; //no error
}
