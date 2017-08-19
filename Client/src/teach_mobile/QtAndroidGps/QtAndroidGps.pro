#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T11:21:39
#
#-------------------------------------------------

QT       += core gui
QT       += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtAndroidGps
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CustomEvent.cpp

HEADERS  += mainwindow.h \
    CustomEvent.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
    android/AndroidManifest.xml \
    android/src/com/mtn/mes/GpsService.java \
    android/src/com/mtn/mes/ExtendsQtNative.java

