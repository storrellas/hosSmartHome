#ifndef SLEEPERTHREAD_H
#define SLEEPERTHREAD_H


class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(msecs);
    }
};


#endif // SLEEPERTHREAD_H
