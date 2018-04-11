#-------------------------------------------------
#
# Project created by QtCreator 2018-01-04T19:26:13
#
#-------------------------------------------------

QT += core websockets network
QT -= gui

CONFIG += c++11

TARGET = Rosco2Garmin
CONFIG += console
CONFIG -= app_bundle

TARGET = Rosco2Garmin
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ./include
VPATH += ./include

target.path = /home/pi/Rosco2Garmin
INSTALLS += target

SOURCES += \
    main.cpp \
    StreamReader.cpp \
    TrafficMath.cpp \
    Translator.cpp

HEADERS += \
    StratuxStreams.h \
    StreamReader.h \
    Translator.h \
