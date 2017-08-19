#-------------------------------------------------
#
# Project created by QtCreator 2014-12-10T07:11:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = useAssets
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
