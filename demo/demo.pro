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
    main.cpp \
    channelwidget.cpp \
    mainmixerwidget.cpp

HEADERS += \
    mainwindow.h \
    channelwidget.h \
    mainmixerwidget.h

FORMS += \
    mainwindow.ui \
    channelwidget.ui \
    mainmixerwidget.ui

RESOURCES += \
    resources.qrc

