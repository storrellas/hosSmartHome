#include "epuckcommunicator.h"

// Address of the ePuck
char EPuckCommunicator::dest[18] = "10:00:E8:AD:79:A8";


/********************************************************************
 Function:	MainWindow
 Overview:  Public Constructor
********************************************************************/
EPuckCommunicator::EPuckCommunicator()
{
    this->connected = false;
    this->logger = LoggerTextEdit::getInstance(0, 0);

}

/********************************************************************
 Function:	writeBuffer
 Overview:  Writes the buffer
********************************************************************/
int EPuckCommunicator::writeBuffer(char *buf, int len){

    // send a message (do not copy the \0 at the end)
    int bytesWritten = send(this->s, buf, len, 0);
    //printf("\nWritten bytes : %d %s\n\n", bytesWritten, buf);
    this->logger->debug(QString("Written bytes %1 %2").arg(bytesWritten).arg(buf), __FUNCTION__, __LINE__);


    return bytesWritten;
}

/********************************************************************
 Function:	writeBuffer
 Overview:  Writes the buffer
********************************************************************/
int EPuckCommunicator::readBuffer(char *buf, int len){

    // read message
    int bytesRead = recv(this->s, buf, len, 0);
    if(bytesRead>0){
        this->logger->debug(QString("Read bytes[%1]:%2").arg(bytesRead).arg(buf), __FUNCTION__, __LINE__);
    }



    return bytesRead;
}



/********************************************************************
 Function:	connectSocket
 Overview:  connects the socket
********************************************************************/
int EPuckCommunicator::connectSocket(){

    struct sockaddr_rc addr;

    // allocate a socket
    this->s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if( this->s < 0 ){
        this->logger->debug("Socket creation error", __FUNCTION__, __LINE__);
    }


    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = 1;
    str2ba( this->dest, &addr.rc_bdaddr );

    int status = ::connect(this->s, (struct sockaddr *)&addr, sizeof(addr));

    return status;
}

/********************************************************************
 Function:	startService
 Overview:  starts the service
********************************************************************/
int EPuckCommunicator::startService(){

    int bytesRead;
    char rcvBuf[5];

    // Creates the connection
    int status = this->connectSocket();
    if(status < 0 ){
        //printf("The system was not able to connect ePuck. Exiting");
        this->logger->debug("The system was not able to connect ePuck. Exiting", __FUNCTION__, __LINE__);

        this->stopService();
        return 0;
    }
    usleep(200);

    int iterations=0;
    while(iterations < 5){

        //printf("Iteration number %d", iterations);
        this->logger->debug(QString("Iteration number %1").arg(iterations), __FUNCTION__, __LINE__);

        // send Reset Message
        this->writeBuffer((char *) "R\r\n",3);


        // Receiving acknowledge from client
        //printf("\nReceving  ... \n");
        this->logger->debug("\nReceving  ... \n", __FUNCTION__,__LINE__);
        bytesRead = this->readBuffer(rcvBuf,3);
//        if( bytesRead > 0 ) {
//            printf("received:%s:%d\n", rcvBuf, bytesRead);
//        }


        // Compare received string
        if( ::memcmp(rcvBuf,"r\r\n", 3) == 0) break;

        iterations++;
        // wait for some time
        usleep(500);

    }

    // Iterations
    if(iterations == 5){
        //printf("Not able to connect ePuck. Exiting code\n");
        this->logger->debug("Not able to connect ePuck. Exiting code", __FUNCTION__, __LINE__);
        this->stopService();
        return 0;
    }

    //printf("INFO: ePuck device found!\n");
    this->logger->debug("ePuck device found!", __FUNCTION__, __LINE__);

    this->connected = true;

    return 1;
}

/********************************************************************
 Function:	sendToggleLed
 Overview:  Send the toggle led command
********************************************************************/
int EPuckCommunicator::sendToogleLed()
{
    int res = this->writeBuffer((char *)"C\r\n",3);
    return res;
}

/********************************************************************
 Function:	sendMoveForward
 Overview:  Sends the move forward movement
********************************************************************/
int EPuckCommunicator::sendMoveForward()
{
    int res = this->writeBuffer((char *)"D,50,400\r\n",10);
    return res;
}

/********************************************************************
 Function:	sendMoveBackward
 Overview:  Sends the move backward movement
********************************************************************/
int EPuckCommunicator::sendMoveBackward()
{
    int res = this->writeBuffer((char *)"D,-50,400\r\n",10);
    return res;
}


/********************************************************************
 Function:	sendTurnLeft
 Overview:  Sends the turn left command
********************************************************************/
int EPuckCommunicator::sendTurnRight()
{
    int res = this->writeBuffer((char *)"G,T,-90\r\n",9);
    return res;
}

/********************************************************************
 Function:	sendTurnLeft
 Overview:  Sends the turn left command
********************************************************************/
int EPuckCommunicator::sendTurnLeft()
{
    int res = this->writeBuffer((char *)"G,T,90\r\n",8);
    return res;
}

/********************************************************************
 Function:	closeSocket
 Overview:  Closes the communication with the ePuck
********************************************************************/
int EPuckCommunicator::stopService(){

    //printf("Closing ePuck socket \n");
    this->logger->debug("Closing ePuck socket \n", __FUNCTION__,__LINE__);
    ::close(this->s);
    this->connected = false;
    return 1;
}



/********************************************************************
 Function:	MainWindow
 Overview:  Public Destructor
********************************************************************/
EPuckCommunicator::~EPuckCommunicator(){
}





