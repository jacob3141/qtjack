QT += core gui
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = bin
TARGET = ear
TEMPLATE = app
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3
CONFIG -= console
CONFIG += flat

INCLUDEPATH += ../libqjackaudio

LIBS += -L../libqjackaudio/lib \
                -lqjackaudio \
                -ljack \
                -lfftw3

SOURCES += \
    mainwindow.cpp \
    main.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

