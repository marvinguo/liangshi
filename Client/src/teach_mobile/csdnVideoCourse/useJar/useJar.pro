#-------------------------------------------------
#
# Project created by QtCreator 2014-09-15T11:15:21
#
#-------------------------------------------------

QT       += core gui androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = useJar
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
        simpleCustomEvent.cpp

HEADERS  += widget.h \
        simpleCustomEvent.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/an/qt/useJar/ExtendsQtNative.java \
    android/src/an/qt/useJar/ExtendsQtWithJava.java

