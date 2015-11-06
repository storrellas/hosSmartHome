#ifndef CWIID_CONNECTOR_H
#define CWIID_CONNECTOR_H

#include <QObject>
#include <QDateTime>

#include <libcwiid/cwiid.h>
#include "loggerTextEdit.h"
#include "configuration.h"

class CwiidConnector : public QObject
{
    Q_OBJECT
public:
    explicit CwiidConnector(QObject *_parent);
    void open();
    void close();
    void cwiid_callback_CPP(cwiid_wiimote_t *wiimote, int mesg_count,
                            cwiid_mesg mesg[], timespec *timestamp);

    void printLogger(QString str);
    bool isConnected();

private:
    LoggerTextEdit *logger;
    unsigned char rpt_mode;
    cwiid_wiimote_t *wiimote;	// wiimote handle
    void set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode);
    int frequencyCallback;
    bool connected;

signals:
    void accelSignal(int x, int y, int z);
    void setAngleFrameSignal(int angleFrame);
    int  executeActionEPuckSignal(int actionEPuck);
    void getCurrentSelectionSignal(int *value);
    void setWiiMoteStatusSignal(QString status);

public slots:
    void accelSlot(int x, int y, int z);

};

#endif // CWIID_CONNECTOR_H
