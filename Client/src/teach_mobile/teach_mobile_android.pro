# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = teach_mobile
DESTDIR = ../../Win32/Release
QT += core network gui widgets xml androidextras
CONFIG += Release c++11
DEFINES += _LiangshiNetwork _LiangshiNoExport
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/Release
OBJECTS_DIR += Release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(teach_mobile.pri)
win32:RC_FILE = teach_mobile.rc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
LIBS += -L$$PWD/../../library/lib/android/Release/ -lHades
ANDROID_EXTRA_LIBS = $$PWD/../../library/lib/android/Release/libHades.so