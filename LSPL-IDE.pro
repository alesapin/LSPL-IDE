#-------------------------------------------------
#
# Project created by QtCreator 2015-09-14T13:41:03
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LSPL-IDE
TEMPLATE = app
CONFIG += c++11 exceptions
LIBS = -llspl

SOURCES += main.cpp\
        mainwindow.cpp \
    centralwidget.cpp \
    basicwidget.cpp

HEADERS  += mainwindow.h \
    centralwidget.h \
    basicwidget.h

include(Text/Text.pri)
include(Interface/Interface.pri)
include(Patterns/Patterns.pri)
include(Engine/Engine.pri)
include(Matches/Matches.pri)
include(Utils/Utils.pri)
RESOURCES += \
    stylesheet.qrc
