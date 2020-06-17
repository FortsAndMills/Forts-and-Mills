#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T13:53:50
#
#-------------------------------------------------

QT       += core gui
QT       += core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11 -g -Wall

TARGET = FortsAndMillsServer
TEMPLATE = app


SOURCES += main.cpp \
    serverclient.cpp \
    server.cpp

HEADERS  += \
    ServerClient.h \
    Server.h
