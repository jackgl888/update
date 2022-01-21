#-------------------------------------------------
#
# Project created by QtCreator 2020-04-18T10:54:51
#
#-------------------------------------------------

QT       += core gui
QT       += core gui  network
QT       += serialport
QT       += xlsx
QT       += sql

//CONFIG += console
RC_ICONS =uup.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testPro
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    tempreture.cpp \
    custom/combdelegate.cpp \
    custom/customdialog.cpp \
    custom/myanimationbtn.cpp \
    needlebed/bedpart.cpp \
    needlebed/histogramview.cpp \
    needlebed/needlebed.cpp \
    needlebed/needlemap.cpp \
    needlebed/traymsg.cpp \
    ocvneedlebed/ocvneedle.cpp \
    single/single.cpp \
    mainWindow/c_widget.cpp \
    mainWindow/content.cpp \
    temperature/adjustsetdialog.cpp \
    temperature/adjusttemp.cpp \
    temperature/commnunicationset.cpp \
    databrowse.cpp \
    communication/connectdlg.cpp \
    communication/myclient.cpp \
    communication/myserial.cpp \
    communication/mysocket.cpp \
    communication/server.cpp \
    communication/serverthread.cpp


HEADERS += \
    tempreture.h \
    paraset.h \
    databrowse.h \
    networks.h \
    communication/connectdlg.h \
    communication/myclient.h \
    communication/myserial.h \
    communication/mysocket.h \
    communication/server.h \
    communication/serverthread.h \
    custom/combdelegate.h \
    custom/customdialog.h \
    custom/myanimationbtn.h \
    database/databasesql.h \
    single/single.h \
    ocvneedlebed/ocvneedle.h \
    mainWindow/c_widget.h \
    mainWindow/content.h \
    needlebed/bedpart.h \
    needlebed/histogramview.h \
    needlebed/needlebed.h \
    needlebed/needlemap.h \
    needlebed/traymsg.h \
    temperature/adjustsetdialog.h \
    temperature/adjusttemp.h \
    temperature/commnunicationset.h \
    struct.h

RESOURCES += \
    res.qrc

