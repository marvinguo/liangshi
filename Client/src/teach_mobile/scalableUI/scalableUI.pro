#-------------------------------------------------
#
# Project created by QtCreator 2014-12-18T22:26:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scalableUI
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    sizeUtil.cpp \
    qDebug2Logcat.cpp

HEADERS  += widget.h \
    sizeUtil.h \
    qDebug2Logcat.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    images.qrc

