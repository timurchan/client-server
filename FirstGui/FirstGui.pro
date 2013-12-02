#-------------------------------------------------
#
# Project created by QtCreator 2013-11-30T02:57:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network
QT += xml

TARGET = FirstGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xmlcommandsparser.cpp

HEADERS  += mainwindow.h \
    xmlcommandsparser.h

FORMS    += mainwindow.ui
FORMS    += tuneUpPanel.ui

RESOURCES += \
    client.qrc
