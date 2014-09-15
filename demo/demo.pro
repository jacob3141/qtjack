QT += core gui opengl
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = bin
TARGET = ear
TEMPLATE = app
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3
CONFIG -= console
CONFIG += flat

# On Windows we need the libraries provided in the 3rdparty folder.
win32 {
    LIBS += -L../3rdparty/fftw/lib \
            -L../3rdparty/jack/lib

    INCLUDEPATH += ../3rdparty/fftw/include \
                   ../3rdparty/jack/include
}

INCLUDEPATH += include \
               ../libear/include \
               ../libear/include/jnoise


LIBS += -L../libear/lib \
                -lear \
                -ljack \
                -lfftw3


SOURCES += \
    src/launcher.cpp \
    src/mainwindow.cpp \
    src/visualizerwidget.cpp

HEADERS += \
    include/mainwindow.h \
    include/visualizerwidget.h

FORMS += \
    ui/mainwindow.ui

RESOURCES += \
    Pics.qrc
