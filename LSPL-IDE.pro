#-------------------------------------------------
#
# Project created by QtCreator 2015-09-14T13:41:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LSPL-IDE
TEMPLATE = app

LIBS = -llspl

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

include(Text/Text.pri)
include(Interface/Interface.pri)
include(Patterns/Patterns.pri)
