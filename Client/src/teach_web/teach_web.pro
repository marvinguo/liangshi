# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = teach_web
DESTDIR = ../../Win32/Release
QT += core network script scripttools webkit axserver axcontainer widgets gui webkitwidgets
CONFIG += release
DEFINES += WIN64 QT_DLL QT_WIDGETS_LIB QT_NETWORK_LIB QT_SCRIPT_LIB QT_WEBKITWIDGETS_LIB _LiangshiNetwork _LiangshiWebkit _LiangshiActiveX
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(teach_web.pri)
win32:RC_FILE = teach_web.rc