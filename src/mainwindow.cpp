#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "configuration.h"
#include "sleeper_thread.h"
#include "util.h"


#define loggerMacro(s) this->logger->debug(s, __FUNCTION__,__LINE__);


// SELECT THE TARGET TO BE SENT THE COMMANDS
#define TARGET_XBEE
//#define TARGET_AMI

#include <QtNetwork>
#include <QApplication>

/********************************************************************
 Function:	**
 Overview:  Commands definition
********************************************************************/

#ifdef TARGET_XBEE

QString MainWindow::lighTurnOn =  "ZBS-110:[00:13:a2:00:40:6a:41:ea]!:INSTRUCTION='SET POW=ON'";
QString MainWindow::lighTurnOff =  "ZBS-110:[00:13:a2:00:40:6a:41:ea]!:INSTRUCTION='SET POW=OFF'";

#endif


#ifdef TARGET_AMI
QString MainWindow::lighTurnOn =  "<Command>" \
                                   "<ID>CM_BCI_6</ID>" \
                                   "<AppID>LIGHT_DD_001</AppID>" \
                                   "<instruction>PowerOn</instruction>" \
                                   "</Command>";

QString MainWindow::lighTurnOff =  "<Command>" \
                                   "<ID>CM_BCI_6</ID>" \
                                   "<AppID>LIGHT_DD_001</AppID>" \
                                   "<instruction>PowerOff</instruction>" \
                                    "</Command>";
#endif

/********************************************************************
 Function:	MainWindow
 Overview:  Public Constructor
********************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Create Ui
    ui = new Ui::MainWindow();
    ui->setupUi(this);


    // Install the handler
    ui->centralwidget->installEventFilter(this);

    // Program Start Log
    ui->consoleTextEdit->appendPlainText(" -- Program start --");

    // Initialise logger after ui creation
    this->logger = LoggerTextEdit::getInstance(0, ui->consoleTextEdit);

    loggerMacro("First message");

    // Device initialisation
    this->lightPower = false;

    // create ePuckObject
    this->ePuckCommunicator = new EPuckCommunicator();

    // create cwiid object
    this->cwiidConnector = new CwiidConnector(0);


    // Set all default color
    this->currentItem = 0;

    // Paint Hex-o-Select
    this->hos = new HoS(0);
    this->hos->enableTurn();
#ifdef TRANSLATE_FONTS
    this->hosContainer = new HoSContainer(ui->hosGroupBox, this->hos, 50);
#else
    this->hosContainer = new HoSContainer(ui->hosGroupBox, this->hos, 5);
#endif
    QVBoxLayout *hosGroupBoxLayout = new QVBoxLayout(ui->hosGroupBox);
    hosGroupBoxLayout->addWidget(hosContainer);

    // Connect cwiidConnector and HoS for setAngleFrame
    QObject::connect(this->cwiidConnector, SIGNAL(setAngleFrameSignal(int)),
                     this->hosContainer, SLOT(setAngleFrameSlot(int)));

    // Connect signals
    QObject::connect(this->cwiidConnector, SIGNAL(executeActionEPuckSignal(int)),
                     this, SLOT(executeActionEPuckSlot(int)));

    // Connect cwiidConnector and HoS for setAngleFrame
    QObject::connect(this->cwiidConnector, SIGNAL(setWiiMoteStatusSignal(QString)),
                     this->hosContainer, SLOT(setWiiMoteStatusSlot(QString)));


//    // Connect signals
//    QObject::connect(this->cwiidConnector, SIGNAL(getCurrentSelectionSignal(int*)),
//                     this->hosContainer, SLOT(getCurrentSelectionSlot(int*)));

    // Debugging purposes
    //this->ePuckConnect();

    // Hide the console initially
    ui->consoleGroupBox->hide();

}

/********************************************************************
 Function:  ~MainWindow
 Overview:  Public Destructor
********************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}


/********************************************************************
 Function:  eventFilter
 Overview:  handles the event from the widgets
********************************************************************/
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    (void)obj;

    // Mouse Control
    if(event->type() == QEvent::MouseButtonRelease){
        QPoint point = QCursor::pos();
        int nButton = ((QMouseEvent*)event)->button();

        loggerMacro(QString("Button %1 mouse pressed. Position at %2,%3").arg(nButton).arg(point.x()).arg(point.y()) )

        // Disable HoS
        loggerMacro("Disable HoS" );
        hos->disableTurn();

        // executes ePuck Action
        this->executeActionEPuckSlot(hos->getCurrentSelection());

        // Enable HoS
        loggerMacro("Enable HoS" );
        hos->enableTurn();

        /*
        loggerMacro("Disable HoS" );
        hos->disableTurn();
        SleeperThread::msleep(1000);
        loggerMacro("Enable HoS" );
        hos->enableTurn();
        /**/


        // Get the event and not propagate
        event->accept();
        return true;
    }


    // Keyboard control
    if(event->type() == QEvent::KeyPress){

        QKeyEvent *k = (QKeyEvent*)event;
        Qt::Key keyPressed = (Qt::Key)k->key();

        if (keyPressed == Qt::Key_Space) {
            loggerMacro("Pressed Space");

            // Disable HoS
            loggerMacro("Disable HoS" );
            hos->disableTurn();

            // executes ePuck Action
            this->executeActionEPuckSlot(hos->getCurrentSelection());

            // Enable HoS
            loggerMacro("Enable HoS" );
            hos->enableTurn();

        } else if (keyPressed == Qt::Key_Alt) {
            loggerMacro("Pressed Alt");

            // Hide console
            if( ui->consoleGroupBox->isHidden() ) ui->consoleGroupBox->show();
            else ui->consoleGroupBox->hide();


        } else if (keyPressed == Qt::Key_AltGr){
            loggerMacro("Pressed Alt Gr");
            this->hosContainer->setAngleFrameQML(45);
        } else if (keyPressed == Qt::Key_Escape){
            qDebug("-- Program exited --");
            // Close connector whether we are connected
            if(cwiidConnector->isConnected() ){
                cwiidConnector->close();
            }

            QApplication::instance()->quit();
        }

        event->accept();
        return true;
    }
    return false;

}

/********************************************************************
 Function:  executeActionEPuck()
 Overview:  executes an action over the ePuck
********************************************************************/
int MainWindow::executeActionEPuckSlot(int actionEPuck){

    loggerMacro(QString("Current Selection is %1").arg(actionEPuck) );

    // Process all events before launching
    processEvents();

    actionEPuck = hosContainer->getCurrentSelectionSlot();

    qDebug() << "ActionEPuck " << actionEPuck;


    switch(actionEPuck){
        case 0:
            this->ePuckUp();
            break;
        case 2:this->ePuckConnect();
            break;
        case 1: this->ePuckLeft();
            break;
        case 3: this->ePuckDown();
            break;
        case 5: this->ePuckRight();
            break;
        case 7:
            break;
        default:
            break;
    }
/**/
    return 1;
}


/********************************************************************
 Function:  getCentralWidget()
 Overview:  Returns the central widget
********************************************************************/
QWidget* MainWindow::getCentralWidget(){
    return ui->centralwidget;
}


/********************************************************************
 Function:  on_ePuckConnectButton_clicked()
 Overview:  Handles the communication with the ePuck
********************************************************************/
void MainWindow::ePuckConnect()
{
    int res;


    loggerMacro("Initialising BT ... ");
    if(!ePuckCommunicator->connected){
        res = ePuckCommunicator->startService();
        if(res == 1){
            loggerMacro("ePuck successfully connected");
        }

    }else{
        res = ePuckCommunicator->stopService();
        if(res == 1){
            loggerMacro("ePuck successfully disconnected");
        }

    }

}

/********************************************************************
 Function:  ePuckUp()
 Overview:  Handles the communication with the ePuck
********************************************************************/
void MainWindow::ePuckUp()
{
    this->logger->debug("Moving ePuckUp Forward", __FUNCTION__, __LINE__);


    if(ePuckCommunicator->connected){
        ePuckCommunicator->sendMoveForward();
    }else{
        this->logger->debug("ePuck is not connected try turn it on", __FUNCTION__, __LINE__);
    }
}

/********************************************************************
 Function:  ePuckLeft()
 Overview:  Handles the communication with the ePuck
********************************************************************/
void MainWindow::ePuckLeft()
{
    this->logger->debug("Moving ePuckUp Left", __FUNCTION__, __LINE__);
    if(ePuckCommunicator->connected){
        ePuckCommunicator->sendTurnLeft();
    }else{
        this->logger->debug("ePuck is not connected try turn it on", __FUNCTION__, __LINE__);
    }
}

/********************************************************************
 Function:  ePuckRight()
 Overview:  Handles the communication with the ePuck
********************************************************************/
void MainWindow::ePuckRight()
{
    this->logger->debug("Moving ePuckUp Right", __FUNCTION__, __LINE__);
    if(ePuckCommunicator->connected){
        ePuckCommunicator->sendTurnRight();
    }else{
        this->logger->debug("ePuck is not connected try turn it on", __FUNCTION__, __LINE__);
    }
}

/********************************************************************
 Function:  ePuckDown()
 Overview:  Handles the communication with the ePuck
********************************************************************/
void MainWindow::ePuckDown()
{
    this->logger->debug("Moving ePuckUp Backwards", __FUNCTION__, __LINE__);
    if(ePuckCommunicator->connected){
        ePuckCommunicator->sendMoveBackward();
    }else{
        this->logger->debug("ePuck is not connected try turn it on", __FUNCTION__, __LINE__);
    }
}


/********************************************************************
 Function:  initialiseWii()
 Overview:  Initialises the Wii Connection
********************************************************************/
void MainWindow::initialiseWii()
{
    loggerMacro("Calling initialise Wii");
    this->cwiidConnector->open();
}

/********************************************************************
 Function:  grabKeyboardCentralWidget()
 Overview:  Makes the central widget to grab the keyboard
********************************************************************/
void MainWindow::grabKeyboardCentralWidget()
{
    ui->centralwidget->grabKeyboard();
}

///********************************************************************
// Function:  on_lightPushButton_clicked()
// Overview:  handler for push button
//********************************************************************/
//void MainWindow::on_lightPushButton_clicked()
//{
//    loggerMacro("Light button clicked");

//    // If turned off, then send turn on
//    QString command = (!this->lightPower)?this->lighTurnOn:this->lighTurnOff;

//    // Create the communicator
//    AmiCommunicator* amiCommunicator = new AmiCommunicator(this);
//    amiCommunicator->sendUDPFrame(command);


//    // Switch variable
//    this->lightPower = ! this->lightPower;
//    QString lightPowerStr = (this->lightPower)?"On":"Off";
//    //this->logger->debug(QString("Power light is %1").arg(lightPowerStr), __FUNCTION__, __LINE__);
//    loggerMacro(QString("Power light is %1").arg(lightPowerStr));

//    // If turned off, next command is turned on
//    lightPowerStr = (!this->lightPower)?"On":"Off";
//    //ui->lightPushButton->setText(QString("Light %1").arg(lightPowerStr));
//}

///********************************************************************
// Function:  on_volUpPushButton_clicked()
// Overview:
//********************************************************************/
//void MainWindow::on_volUpPushButton_clicked()
//{
//    loggerMacro("Function not implemented");
//}

///********************************************************************
// Function:  on_volDownPushButton_clicked()
// Overview:
//********************************************************************/
//void MainWindow::on_volDownPushButton_clicked()
//{
//    loggerMacro("Function not implemented");
//}

///********************************************************************
// Function:  on_chUpPushButton_clicked()
// Overview:
//********************************************************************/
//void MainWindow::on_chUpPushButton_clicked()
//{
//    loggerMacro("Function not implemented");
//}

///********************************************************************
// Function:  on_chDownPushButton_clicked()
// Overview:
//********************************************************************/
//void MainWindow::on_chDownPushButton_clicked()
//{
//    loggerMacro("Function not implemented");
//}

///********************************************************************
// Function:  on_tvPushButton_clicked()
// Overview:
//********************************************************************/
//void MainWindow::on_tvPushButton_clicked()
//{
//    loggerMacro("Function not implemented");
//}
