#-------------------------------------------------
#
# Project created by QtCreator 2014-04-10T14:38:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MediaAndroid
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

LIBS += -L$$_PRO_FILE_PWD_/../../library/lib/android/Debug -lcollect

ANDROID_EXTRA_LIBS = ../../library/lib/android/Debug/libcollect.so
