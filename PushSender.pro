#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T23:05:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = PushSender
TEMPLATE = app

RC_ICONS = pushsender.ico

win32:LIBS += C:\Libraries\lib\libeay32.lib C:\Libraries\lib\ssleay32.lib C:\Libraries\build-qca-master-Desktop_Qt_5_7_0_MSVC2013_32bit-Default\lib\qca-qt5d.lib
win32:INCLUDEPATH += C:\Libraries\include C:\Libraries\qca-master\include\QtCrypto

mac:LIBS += -L /Users/tulio/Downloads/openssl-OpenSSL_1_0_2g -lssl -lcrypto
mac:INCLUDEPATH += /Users/tulio/Downloads/openssl-OpenSSL_1_0_2g/include

SOURCES += main.cpp\
        mainwindow.cpp \
    apnsconnector.cpp \
    gcmconnector.cpp

HEADERS  += mainwindow.h \
    apnsconnector.h \
    gcmconnector.h

FORMS    += mainwindow.ui
