#ifndef FLASHERTHREAD_H
#define FLASHERTHREAD_H

#include "Flasher.h" // Flasher
#include <QThread>   // QThread

class FlasherThread : public QThread
{
  private:
    Flasher *flasher;

  public:
    FlasherThread(Flasher *flasher);

    void run();
};

#endif // FLASHERTHREAD_H