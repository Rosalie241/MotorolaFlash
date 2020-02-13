#include "FlasherThread.h"

FlasherThread::FlasherThread(Flasher *flasher)
{
    this->flasher = flasher;
}

void FlasherThread::run()
{
    flasher->Flash();
}