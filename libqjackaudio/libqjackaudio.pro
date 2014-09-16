TEMPLATE = lib
CONFIG += staticlib flat
TARGET = qjackaudio
DESTDIR = lib
OBJECTS_DIR = obj
MOC_DIR = moc
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3

SOURCES += \
    qequalizer.cpp \
    qfftw.cpp \
    qjackclient.cpp \
    qjackport.cpp

HEADERS += \
    QAudioProcessor \
    qaudioprocessor.h \
    QEqualizer \
    qequalizer.h \
    qfftw.h \
    QJackClient \
    qjackclient.h \
    QJackPort \
    qjackport.h \
    QFFTW \
    qsemaphorelocker.h \
    QSemaphoreLocker
