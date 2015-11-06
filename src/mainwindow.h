#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "amicommunicator.h"
#include "epuckcommunicator.h"
#include "loggerTextEdit.h"
#include "cwiidconnector.h"
#include "hoscontainer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget* getCentralWidget();
    void logInfo(QtMsgType type, QString msg, QString function, int line);

    LoggerTextEdit *logger;
    Ui::MainWindow *ui;

    void initialiseWii();
    void grabKeyboardCentralWidget();

    enum ActionEPuck { Up, Down, Left, Right, Connect, Disconnect};

private:
    static QString lighTurnOn;
    static QString lighTurnOff;
    EPuckCommunicator* ePuckCommunicator;
    CwiidConnector *cwiidConnector;
    HoSContainer *hosContainer;

    bool lightPower;

    int currentItem;
    HoS *hos;


    // Functions
    void ePuckConnect();
    void ePuckUp();
    void ePuckLeft();
    void ePuckRight();
    void ePuckDown();

public slots:
    int executeActionEPuckSlot(int actionEPuck);



private slots:


//   Pikkerton code
//    void on_lightPushButton_clicked();
//    void on_volUpPushButton_clicked();
//    void on_volDownPushButton_clicked();
//    void on_chUpPushButton_clicked();
//    void on_chDownPushButton_clicked();
//    void on_tvPushButton_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
