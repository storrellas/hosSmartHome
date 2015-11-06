#include "hoscontainer.h"

#include <QtGui>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedLayout>

#define QDECLARATIVE

#ifdef QDECLARATIVE
    #include <QDeclarativeView>
    #include <QDeclarativeEngine>
    #include <QDeclarativeComponent>
    #include <QDeclarativeItem>
    #include <QDeclarativeProperty>
#else
    #include <QtQuick/QtQuick>
    #include <QtGui/QtGui>
#endif

#define SELECTION_NUMBER 8
#define FULL_CIRCLE 360

#define loggerMacro(s) this->logger->debug(s, __FUNCTION__,__LINE__);

/********************************************************************
 Function:  HoSContainer
 Overview:  Public constructor
********************************************************************/
HoSContainer::HoSContainer(QWidget *parent, HoS *_hos, int _fontSize) :
    QWidget(parent),hos(_hos), fontSize(_fontSize)
{

    // Initialise logger after ui creation
    this->logger = LoggerTextEdit::getInstance(0, 0);

    // Configure hoscontainer
    setObjectName(QString::fromUtf8("hosContainer"));
    setStyleSheet(QString::fromUtf8("	background-color: #717D7D;\n"
                                    "     border-radius: 10px; \n"
                                    "     margin-top: 1ex;\n"));

    //QVBoxLayout *vbox = new QVBoxLayout(this);


    QStackedLayout *stackedLayout = new QStackedLayout(this);
#ifdef QDECLARATIVE
    // Create HoS Arrow by QML
    QDeclarativeView *qmlView = new QDeclarativeView(this);
    qmlView->setAttribute(Qt::WA_TranslucentBackground);
    qmlView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    qmlView->setSource(QUrl("qrc:/qml/Hos"));
    qmlView->setGeometry(0,0, this->width(), this->height());


    // Add qmlView
    //vbox->addWidget( qmlView );
    stackedLayout->addWidget(qmlView);

    // Create signal to communicate with QML
    hosObject = dynamic_cast<QObject*>(qmlView->rootObject());
    QObject::connect(this, SIGNAL(setAngleFrameQMLSignal(QVariant)), hosObject, SLOT(setAngleFrameQMLSlot(QVariant)));
    QObject::connect(hosObject, SIGNAL(getAngleFrameQMLSignal(QString)), this, SLOT(getAngleFrameQMLSlot(QString)));
    QObject::connect(hosObject, SIGNAL(handleUpdateAngleQMLSignal(QString)), this, SLOT(handleUpdateAngleQMLSlot(QString)));

    QObject::connect(this, SIGNAL(setWiiMoteStatusQMLSignal(QVariant)), hosObject, SLOT(setWiiMoteStatusQMLSlot(QVariant)));


/*
    QVariant aux = QDeclarativeProperty::read(hosObject, "currentHexagon");
    aux.toInt();
/**/
#else
    QWidget *widget = new QWidget();
    QQuickView *view = new QQuickView(widget->windowHandle());
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl(QCoreApplication::applicationDirPath() + "/qml/hosArrow.qml"));

    // Add qmlView
    vbox->addWidget( widget );

    // Create signal to communicate with QML
    QObject *rootObject = dynamic_cast<QObject*>(view->rootObject());
    QObject::connect(this, SIGNAL(setAngleFrameQMLSignal(QVariant)), rootObject, SLOT(setAngleFrameQMLSlot(QVariant)));
    QObject::connect(rootObject, SIGNAL(getAngleFrameQMLSignal(QString)), this, SLOT(getAngleFrameQMLSlot(QString)));
#endif

/*
    // Widget-based HoS
    this->hos = new HoS(0);
    vbox->addWidget( this->hos );
    this->hos->enableTurn();
/**/

    // Initialise angle frame
    this->angleFrame = 0;

}

/********************************************************************
 Function:  setAngleFrameQML
 Overview:  Changes the angle with which the arrow is moving
********************************************************************/
bool HoSContainer::setAngleFrameQML(int increment){

    if( this->angleFrame != increment){
        this->angleFrame = increment;
        emit setAngleFrameQMLSignal(QVariant(increment));
        loggerMacro("Setting new AngleFrame in QML");
    }


    return true;
}

/********************************************************************
 Function:  setAngleFrameQML
 Overview:  Changes the angle with which the arrow is moving
********************************************************************/
void HoSContainer::getAngleFrameQMLSlot(QString angleFrame){

    loggerMacro(QString("Angle frame is %1").arg(angleFrame));
}

/********************************************************************
 Function:  setAngleFrameSlot
 Overview:  Changes the angle with which the arrow is moving
********************************************************************/
void HoSContainer::setAngleFrameSlot(int angleFrame){
    this->setAngleFrameQML(angleFrame);
}

#define SELECTION_NUMBER 8
#define FULL_CIRCLE 360

/********************************************************************
 Function:  updateAngleSlot
 Overview:  Updates the current value of the angle
********************************************************************/
void HoSContainer::handleUpdateAngleQMLSlot(QString currentSelection){
    //loggerMacro(QString("Update in Angle value -> %1").arg(currentSelection));
    this->currentAngle = currentSelection.toInt();

}

/********************************************************************
 Function:  getCurrentSelection
 Overview:  returns the current selected option
********************************************************************/
int HoSContainer::getCurrentSelectionSlot(){

    /*
        int anglePerSelection = FULL_CIRCLE/SELECTION_NUMBER;
        int selection =(int) this->currentAngle/anglePerSelection;
    /**/


    QVariant aux = QDeclarativeProperty::read(hosObject, "currentHexagon");
    return aux.toInt();
/*
    qDebug() << "Current HexHosConatainer: " << aux << " Working " << *value;

    //return currentHexagonInt;
    return;
/**/
    return 0;
}

/********************************************************************
 Function:  setWiiMoteEnabled
 Overview:  Sets the wii s
********************************************************************/
bool HoSContainer::setWiiMoteStatusSlot(QString status){

    // Communicate with QML
    emit setWiiMoteStatusQMLSignal(status);
    return true;
}

