#ifndef HOSCONTAINER_H
#define HOSCONTAINER_H

#include <QWidget>
#include <QStyleOption>

#include "loggerTextEdit.h"
#include "hos.h"

class HoSContainer : public QWidget
{
    Q_OBJECT
public:
    HoSContainer(QWidget *parent = 0, HoS *_hos = 0, int _fontSize = 0);
    bool setAngleFrameQML(int increment);

private:
    HoS *hos;
    QObject *hosObject;

    int fontSize;
    LoggerTextEdit *logger;
    int angleFrame;
    int currentAngle;

signals:
    void setAngleFrameQMLSignal(QVariant value);
    void setWiiMoteStatusQMLSignal(QVariant status);

public slots:
    void getAngleFrameQMLSlot(QString angleFrame);
    void setAngleFrameSlot(int angleFrame);
    void handleUpdateAngleQMLSlot(QString currentSelection);
    int getCurrentSelectionSlot();
    bool setWiiMoteStatusSlot(QString status);


protected:

};

#endif // HOSCONTAINER_H
