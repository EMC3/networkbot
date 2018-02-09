#-------------------------------------------------
#
# Project created by QtCreator 2018-02-08T16:41:31
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nmapbot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    dataman/cbytebuf.cpp \
    logger/log.cpp \
    logger/logdisplay.cpp \
    logger/loggerbackend.cpp \
    config.cpp \
    managedchat.cpp \
    command.cpp

HEADERS += \
    logger/log.h \
    logger/logdisplay.h \
    logger/loggerbackend.h \
    dataman/cbytebuf.h \
    dataman/yaml2qstring.h \
    config.h \
    managedchat.h \
    command.h

FORMS += \
    logger/logdisplay.ui


LIBS += -lyaml-cpp -ltgbot -lz -lcrypto -lssl -lboost_system
