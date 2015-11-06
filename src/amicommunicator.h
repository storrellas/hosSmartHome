#ifndef AMICOMMUNICATOR_H
#define AMICOMMUNICATOR_H

#include <QObject>
#include <QtNetwork>

#include "loggerTextEdit.h"

class AmiCommunicator : public QObject
{
    Q_OBJECT

private:
    LoggerTextEdit *logger;
    // Establish connection with the echo server
    QUdpSocket *udpSocket;
public:
    explicit AmiCommunicator(QObject *_parent = 0);
    ~AmiCommunicator();
    void sendUDPFrame(QString command);

private slots:
};

#endif // AMICOMMUNICATOR_H
