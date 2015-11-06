#include <QtWidgets/QApplication>
#include "mainwindow.h"

#include <QPushButton>

#include "epuckcommunicator.h"
#include "cwiidconnector.h"

#include "configuration.h"


/********************************************************************
 Function:	translateFonts
 Overview:  translates the font for a specific widget
********************************************************************/
void translateFonts( QWidget *ui ){
    QFont font = QFont(ui->font());
    //font.setPointSize( font.pointSize() * 3 );
    font.setPointSize( 100 );
    ui->setFont(font);
}


/********************************************************************
 Function:	translateFontSize
 Overview:  translates the font for a widget and its children
********************************************************************/
void translateFontSize( QWidget *ui )
{
    QObjectList l;
    QObject *w;

    if( !ui ) return;

    translateFonts(ui);
    l = ui->children();

    foreach (w, l) {
        QWidget *ww = dynamic_cast<QWidget *>(w);
        if (ww) {
            translateFontSize( ww );
        }


    }
}



/********************************************************************
 Function:	main
 Overview:  main function entry point
********************************************************************/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    qDebug("-- Program started --");

    // Remove cursor
#ifdef REMOVE_CURSOR
    #ifdef Q_WS_QWS
        QWSServer::setCursorVisible( false );
    #endif
#endif

    // translates the fonts
#ifdef TRANSLATE_FONTS
    translateFontSize( w.getCentralWidget() );
#endif

    // Set undecorated window and maximised
#ifdef SET_UNDECORATED
    w.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint); //Set window to fixed size
    w.setWindowFlags(Qt::CustomizeWindowHint); //Set window with no title bar
    w.setWindowFlags(Qt::FramelessWindowHint); //Set a frameless window
    w.showMaximized();
#else
    w.show();
#endif

    // Central widget monitors the keyboard activity
    w.grabKeyboardCentralWidget();

    // Initialises wii connection
    w.initialiseWii();



    return a.exec();
}
