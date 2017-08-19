#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30T09:31:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PaintShapes
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    shapeSelector.cpp \
    shapeFactory.cpp

HEADERS  += widget.h \
    shapeSelector.h \
    shapeFactory.h \
    shapeType.h

RESOURCES += \
    images.qrc
