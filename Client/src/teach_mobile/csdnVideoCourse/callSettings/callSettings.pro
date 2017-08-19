#-------------------------------------------------
#
# Project created by QtCreator 2014-12-14T07:07:40
#
#-------------------------------------------------

QT       += core gui
QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = callSettings
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/an/qt/callSettings/ExtendsQtWithJava.java

