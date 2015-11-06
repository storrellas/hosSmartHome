#ifndef LOGGERTEXTEDIT_H
#define LOGGERTEXTEDIT_H

#include <QObject>
#include <QtWidgets/QPlainTextEdit>

class LoggerTextEdit : public QObject
{
    Q_OBJECT

private:
    // Attributes
    static LoggerTextEdit* loggerTextEdit;
    static bool instanceFlag;

    // Functions
    void appendMessage(QtMsgType type, QString msg, QString function, int line);
    explicit LoggerTextEdit(QObject *_parent, QPlainTextEdit* _consoleTextEdit);


public:
    ~LoggerTextEdit();

    // Attributes
    QPlainTextEdit* consoleTextEdit;

    // Functions
    void debug(QString msg, QString function, int line);
    void warning(QString msg, QString function, int line);
    void critical(QString msg, QString function, int line);
    void fatal(QString msg, QString function, int line);

    static LoggerTextEdit* getInstance(QObject *_parent, QPlainTextEdit* _consoleTextEdit);


};

#endif // LOGGERTEXTEDIT_H
