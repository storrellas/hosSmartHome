#ifndef UTIL_H
#define UTIL_H

#include <QCoreApplication>

#define processEvents()   QCoreApplication::processEvents(QEventLoop::AllEvents, 100); // Execute all events before connecting

#include <QDebug>


#endif // UTIL_H
