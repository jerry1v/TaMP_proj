QT += core gui widgets

CONFIG += c++17
CONFIG -= app_bundle

TARGET = AuthClient

# Prevent windows.h from pulling in winsock 1.x before winsock2
DEFINES += WIN32_LEAN_AND_MEAN

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    adminpanel.cpp \
    client.cpp

HEADERS += \
    mainwindow.h \
    adminpanel.h \
    client.h

LIBS += -lws2_32
