QT -= gui
QT -= core

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++17

TARGET = AuthServer

DEFINES += WIN32_LEAN_AND_MEAN

SOURCES += \
    main.cpp \
    databasemanager.cpp

HEADERS += \
    databasemanager.h

LIBS += -lws2_32
