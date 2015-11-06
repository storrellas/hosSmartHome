#ifndef EPUCKCOMMUNICATOR_H
#define EPUCKCOMMUNICATOR_H


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>


#include <bluetooth/bluetooth.h>
#include "bluetooth/rfcomm.h"

#include "loggerTextEdit.h"

class EPuckCommunicator
{

private:
    LoggerTextEdit* logger;

    static char dest[18];
    int s;

    int connectSocket();
    int writeBuffer(char *buf, int len);
    int readBuffer(char *buf, int len);


public:
    explicit EPuckCommunicator();
    ~EPuckCommunicator();
    int stopService();
    int startService();


    int sendToogleLed();
    int sendMoveForward();
    int sendMoveBackward();
    int sendTurnRight();
    int sendTurnLeft();

    bool connected;

};

#endif // EPUCKCOMMUNICATOR_H
