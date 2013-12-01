#-------------------------------------------------
#
# Project created by QtCreator 2013-11-29T20:33:27
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = TimServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    timudpserver.cpp \
    timtcpserver.cpp

HEADERS += \
    timudpserver.h \
    timtcpserver.h
