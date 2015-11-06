#include "loggerTextEdit.h"

#include <QDateTime>

#define LOG_WITH_TIME

LoggerTextEdit* LoggerTextEdit::loggerTextEdit = 0;
bool LoggerTextEdit::instanceFlag = false;

/********************************************************************
 Function:  LoggerTextEdit
 Overview:  Private Constructor
********************************************************************/
LoggerTextEdit::LoggerTextEdit(QObject *_parent, QPlainTextEdit* _consoleTextEdit) :
    QObject(_parent)
{
    if(_consoleTextEdit != 0){
        consoleTextEdit =_consoleTextEdit;
    }

}

/********************************************************************
 Function:  ~LoggerTextEdit
 Overview:  Public Destructor
********************************************************************/
LoggerTextEdit::~LoggerTextEdit()
{
    this->instanceFlag = false;
}

/********************************************************************
 Function:  getInstance
 Overview:  Returns and instance of the logger
********************************************************************/
LoggerTextEdit* LoggerTextEdit::getInstance(QObject *_parent, QPlainTextEdit* _consoleTextEdit){
    if(! instanceFlag)
        {
            loggerTextEdit = new LoggerTextEdit(_parent, _consoleTextEdit);
            instanceFlag = true;
            return loggerTextEdit;
        }
        else
        {
            return loggerTextEdit;
        }
}

/********************************************************************
 Function:  **
 Overview:  Specific logging functions
********************************************************************/
void LoggerTextEdit::debug(QString msg, QString function, int line){
    this->appendMessage(QtDebugMsg, msg, function, line);
}
void LoggerTextEdit::warning(QString msg, QString function, int line){
    this->appendMessage(QtWarningMsg, msg, function, line);
}
void LoggerTextEdit::critical(QString msg, QString function, int line){
    this->appendMessage(QtCriticalMsg, msg, function, line);
}
void LoggerTextEdit::fatal(QString msg, QString function, int line){
    this->appendMessage(QtFatalMsg, msg, function, line);
}


/********************************************************************
 Function:  appendMessage
 Overview:  creates a log message according to QtMsgType
********************************************************************/
void LoggerTextEdit::appendMessage(QtMsgType type, QString msg, QString function, int line)
{

    QString logStr = "";


#ifdef LOG_WITH_TIME
    QTime time = QTime::currentTime();
    logStr = time.toString() + " | ";
#endif

    //in this function, you can write the message to any stream!
    switch (type) {
    case QtDebugMsg:
        logStr += "Debug | ";
        break;
    case QtWarningMsg:
        logStr += "Warning | ";
        break;
    case QtCriticalMsg:
        logStr += "Critical | ";
        break;
    case QtFatalMsg:
        logStr += "Fatal |";
        abort();
        break;
    default:
        break;
    }

    logStr +=  function + " | " + QString("%1").arg(line) + " | " + msg;


    // Append to TextEdit
    if(this->consoleTextEdit != 0 ){
        this->consoleTextEdit->appendPlainText(logStr);
    }else{
        qDebug(" Not console was assigned");
        exit(0);
    }

    // Scroll to bottom
    consoleTextEdit->ensureCursorVisible();
}
