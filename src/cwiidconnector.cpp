#include "cwiidconnector.h"

#include "util.h"

// MACRO Section
#define toggle_bit(bf,b)	\
        (bf) = ((bf) & b)		\
               ? ((bf) & ~(b))	\
               : ((bf) | (b))
#define loggerMacro(s) this->logger->debug(s, __FUNCTION__,__LINE__);



/********************************************************************
 Extern C Functions
********************************************************************/
extern "C"{

    CwiidConnector *cwiidConnector_C;
    void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                    union cwiid_mesg mesg[], struct timespec *timestamp)
    {
        cwiidConnector_C->cwiid_callback_CPP(wiimote, mesg_count, mesg, timestamp);
    }


    void err(cwiid_wiimote_t *wiimote, const char *s, va_list ap)
    {
        if (wiimote) printf("%d:", cwiid_get_id(wiimote));
        else printf("-1:");
        vprintf(s, ap);
        printf("\n");
    }

}


/********************************************************************
 Function:	MainWindow
 Overview:  Public Constructor
********************************************************************/
CwiidConnector::CwiidConnector(QObject *_parent)
{

    // Initialise logger after ui creation
    this->logger = LoggerTextEdit::getInstance(0, NULL);

    // Member initialisation
    this->rpt_mode = 0;
    this->frequencyCallback = 0;

    loggerMacro("This is working frin CwiddConnector");


    // Initialise object for communication
    cwiidConnector_C = this;

    // Initially we are not connected
    this->connected = false;

}

/********************************************************************
 Function:	CwiidConnector::open
 Overview:  Opens the communication
********************************************************************/
void CwiidConnector::open()
{

    bdaddr_t bdaddr;	// bluetooth device address

    bdaddr = *BDADDR_ANY;
    //cwiid_err_t err;


    cwiid_set_err(err);
    cwiid_mesg_callback_t cwiid_callback;

    // Connect to the wiimote
    loggerMacro("Put Wiimote in discoverable mode now (press 1+2)...\n");
    processEvents(); // Execute all events before connecting

    if (!(wiimote = cwiid_open(&bdaddr, 0))) {
            loggerMacro("Unable to connect to wiimote\n");
            emit setWiiMoteStatusSignal("disabled");
            return;
    }else{
        this->connected = true;
    }

    if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) {
            loggerMacro("Unable to set message callback\n");
    }

    // Enable cwiid handle
    if (cwiid_enable(this->wiimote, CWIID_FLAG_MESG_IFC)) {
        loggerMacro("Error enabling messages\n");
    }
/*
    // Set accelerometer report mode
    toggle_bit(this->rpt_mode, CWIID_RPT_ACC);
    set_rpt_mode(this->wiimote, this->rpt_mode);
/**/

    // Enable Joypad & Buttons
    toggle_bit(rpt_mode, CWIID_RPT_BTN);
    set_rpt_mode(wiimote, rpt_mode);

    // Connect signal for callback message
    QObject::connect(this, SIGNAL(accelSignal(int, int, int)),
                     this, SLOT(accelSlot(int, int, int)));

    loggerMacro("Open connection successful");
    qDebug() << "Open connection successful";

    // Emit signal to update the interface
    emit setWiiMoteStatusSignal("enabled");

}

/********************************************************************
 Function:	CwiidConnector::close
 Overview:  Close the communication
********************************************************************/
void CwiidConnector::close()
{
    loggerMacro("Closing the WiiMote channel");
    processEvents(); // Execute all events before connecting
    if (cwiid_close(this->wiimote)) {
            loggerMacro("Unable to close channel\n");
            return;
    }else{
        // Emit signal to update the interface
        this->connected = false;
        emit setWiiMoteStatusSignal("disabled");
        return;
    }
}


/********************************************************************
 Function:	CwiidConnector::cwiid_callback_CPP
 Overview:  Callback function from messages coming from Wii
********************************************************************/
void CwiidConnector::cwiid_callback_CPP(cwiid_wiimote_t *wiimote, int mesg_count,
                                        union cwiid_mesg mesg[], struct timespec *timestamp){

    int i;
    int currentSelection = 0;

    char c;
    for (i=0; i < mesg_count; i++)
    {
        switch (mesg[i].type) {
            case CWIID_MESG_STATUS:
                break;

            case CWIID_MESG_BTN:
                qDebug("Button Report: %.4X\n", mesg[i].btn_mesg.buttons);

                if(mesg[i].btn_mesg.buttons != 0){
                    emit executeActionEPuckSignal(1);
                }else{
                    qDebug("Button was released");
                }

                break;

            case CWIID_MESG_ACC:
                this->frequencyCallback++;
                if((this->frequencyCallback % 100) == 0){

                    int x = mesg[0].acc_mesg.acc[CWIID_X];
                    int y = mesg[0].acc_mesg.acc[CWIID_Y];
                    int z = mesg[0].acc_mesg.acc[CWIID_Z];

                    // Emit signal for logger
                    emit accelSignal(x, y, z);
                }
                break;
            case CWIID_MESG_IR     :break;
            case CWIID_MESG_NUNCHUK:break;
            case CWIID_MESG_CLASSIC:break;
            case CWIID_MESG_ERROR  :break;
            default:
                qDebug() << "Unknown Report";
                break;

        }
    }

/*
    this->frequencyCallback++;
    if((this->frequencyCallback % 100) == 0){

        int x = mesg[0].acc_mesg.acc[CWIID_X];
        int y = mesg[0].acc_mesg.acc[CWIID_Y];
        int z = mesg[0].acc_mesg.acc[CWIID_Z];

        // Emit signal for logger
        emit accelSignal(x, y, z);
    }
/**/
    qDebug() << "Event in console";

}

/********************************************************************
 Function:	CwiidConnector::set_rpt_mode
 Overview:  Sets the report mode
********************************************************************/
void CwiidConnector::set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode)
{
    if (cwiid_set_rpt_mode(wiimote, rpt_mode)) {
        loggerMacro("Error setting report mode\n");
    }
}


/********************************************************************
 Function:	CwiidConnector::set_rpt_mode
 Overview:  Sets the report mode
********************************************************************/
void CwiidConnector::accelSlot(int x, int y, int z)
{
    QString str = QString("Acc Report: x=%1, y=%2, z=%3 \n").arg(x).arg(y).arg(z);
    // Top : 147 ; Botton : 97
    if( y > 122 ){
        str += " -> clockwise";
        emit setAngleFrameSignal(10);
    }else{
        str += " -> anticlockwise";
        emit setAngleFrameSignal(-10);
    }
    loggerMacro(str);
}

/********************************************************************
 Function:	CwiidConnector::isConnected
 Overview:  returns connected variable
********************************************************************/
bool CwiidConnector::isConnected()
{
    return this->connected;
}
