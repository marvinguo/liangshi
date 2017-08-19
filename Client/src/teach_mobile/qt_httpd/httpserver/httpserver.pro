TEMPLATE = app
QT += network xml
QT -= gui
TARGET = httpserver
CONFIG += console
CONFIG -= app_bundle

win32 {
  LIBS += -lws2_32
}

HTTP = ./http
INCLUDEPATH += . \
    $$HTTP \
    $$HTTP/http-parser \
    responders \
    ./utils

DEFINES += SERVER_POLLING_INTERVAL=10

# Input
HEADERS += httpserver.h \
           responders/responder.h \
           responders/responderInterface.h \
           responders/responderManager.h \
           http/httpConnection.h \
           http/httpRequest.h \
           http/httpResponse.h \
    utils/log.h \
    utils/formatHelper.h \
    responders/worker.h \
    serverconfig.h \
    responders/responderDebugHelper.h \
    utils/workerThreadTemplate.h \
    globalInit.h \
    utils/networkAccessHelper.h \
    responders/cacheState.h \
    utils/timeMeasure.h \
    utils/asyncLog.h \
    responders/privHttpHeader.h \
    utils/simpleCustomEvent.h

SOURCES += httpserver.cpp \
           main.cpp \
           responders/responder.cpp \
           responders/responderManager.cpp \
           http/httpConnection.cpp \
           http/httpRequest.cpp \
           http/httpResponse.cpp \
    utils/log.cpp \
    responders/worker.cpp \
    serverconfig.cpp \
    responders/responderDebugHelper.cpp \
    globalInit.cpp \
    utils/networkAccessHelper.cpp \
    utils/asyncLog.cpp \
    utils/simpleCustomEvent.cpp
