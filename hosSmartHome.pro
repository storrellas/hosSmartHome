# -------------------------------------------------
# Project created by QtCreator 2012-06-26T08:07:13
# -------------------------------------------------
# Configuration of application
QT += core \
    gui \
    network \
    widgets \
    quick \
    declarative

# QT += core gui network widgets quick
TEMPLATE = app
target.path = /root/widgetTest
TARGET = hosSmartHome
INSTALLS += target
CONFIG += x86

# CONFIG += RPI
# Source files
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/loggerTextEdit.cpp \
    src/amicommunicator.cpp \
    src/epuckcommunicator.cpp \
    src/hos.cpp \
    src/hoscontainer.cpp \
    src/cwiidconnector.cpp
HEADERS += src/mainwindow.h \
    src/loggerTextEdit.h \
    src/amicommunicator.h \
    src/epuckcommunicator.h \
    src/hos.h \
    src/configuration.h \
    src/sleeper_thread.h \
    src/hoscontainer.h \
    src/cwiidconnector.h \
    src/util.h
FORMS += src/mainwindow.ui
OTHER_FILES += qml/HosArrow.qml \
    qml/HosInner.qml \
    qml/HosHexagon.qml \
    qml/HosInner.qml \
    qml/HosContainer.qml \
    qml/Hos.qml

# Destination directories
DESTDIR = .
OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj
UI_DIR = obj

# Include and library
INCLUDEPATH += $$PWD/src/ \
    $$PWD/src/
DEPENDPATH += $$PWD/src/ \
    $$PWD/src/
QMAKE_CXXFLAGS += -fpermissive
RESOURCES += hosSmartHomeResource.qrc

# libraries for RPI
RPI { 
    LIBS += -L$$PWD/lib/ \
        -lbluetooth \
        -L$$PWD/lib/libcwiid \
        -lcwiid \
        -lrt
    message("Makefile created for RPI")
}

# libraries for x86
x86 { 
    LIBS += -lbluetooth \
        -L$$PWD/lib/libcwiid_x86 \
        -lcwiid \
        -lrt
    message("Makefile created for x86")
}
