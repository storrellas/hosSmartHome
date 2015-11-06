#include "amicommunicator.h"

/*
#define REMOTE_IP "192.168.1.10"
#define REMOTE_PORT 8050
*/

/*
#define REMOTE_IP "172.20.41.103"
#define REMOTE_PORT 8050
*/


#define REMOTE_IP "172.20.41.104"
#define REMOTE_PORT 7050


/********************************************************************
 Function:	MainWindow
 Overview:  Public Constructor
********************************************************************/
AmiCommunicator::AmiCommunicator(QObject *_parent) : QObject(_parent)
{
    this->logger = LoggerTextEdit::getInstance(0, 0);

    this->logger->debug("Initialisation of socket", __FUNCTION__, __LINE__);
}


/********************************************************************
 Function:	MainWindow
 Overview:  Public Destructor
********************************************************************/
AmiCommunicator::~AmiCommunicator()
{

}

/********************************************************************
 Function:	sendCommand
 Overview:  Public Constructor
********************************************************************/
void AmiCommunicator::sendUDPFrame(QString command)
{

    // Establish connection with the echo server
    QUdpSocket *udpSocket = new QUdpSocket(this);

    udpSocket->connectToHost(REMOTE_IP,REMOTE_PORT);
    if( udpSocket->waitForConnected(3000)){

        logger->debug("Sending information ...", __FUNCTION__, __LINE__);
        logger->debug(command, __FUNCTION__, __LINE__);

        qint64 writeBytes = udpSocket->write( command.toLatin1() );

        logger->debug(QString("Written bytes %1").arg(writeBytes), __FUNCTION__, __LINE__);

        /*
        udpSocket->waitForReadyRead(5000);

        logger->debug(QString("Bytes to receive ... %1").arg(udpSocket->bytesAvailable()), __FUNCTION__, __LINE__);

        QByteArray array = udpSocket->readAll();
        logger->debug(QString("Read ... %1").arg(QString(array)), __FUNCTION__, __LINE__);
        /**/

    }

    udpSocket->close();
    // That is a workaround
    delete udpSocket;

}
