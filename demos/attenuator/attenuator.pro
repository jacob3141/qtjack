QT       += core

QT       -= gui

TARGET = attenuator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += ../..

LIBS += -L../../lib -lqjack -ljack -ljacknet
