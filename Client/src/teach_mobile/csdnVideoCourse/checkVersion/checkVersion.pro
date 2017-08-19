#-------------------------------------------------
#
# Project created by QtCreator 2014-12-14T08:54:19
#
#-------------------------------------------------

QT       += core gui
QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = checkVersion
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qDebug2Logcat.cpp

HEADERS  += widget.h \
    qDebug2Logcat.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/an/qt/checkVersion/ExtendsQtWithJava.java \
    android/src/an/qt/checkVersion/ExtendsQtNative.java

