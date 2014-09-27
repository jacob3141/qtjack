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
    qjackport.cpp \
    qsamplebuffer.cpp \
    qcompressor.cpp \
    qunits.cpp \
    qnoisegate.cpp \
    qamplifier.cpp \
    qsignalgenerator.cpp \
    qequalizercontrol.cpp

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
    qsamplebuffer.h \
    QSampleBuffer \
    qdigitalfilter.h \
    QDigitalFilter \
    qcompressor.h \
    QCompressor \
    qunits.h \
    QUnits \
    qnoisegate.h \
    QNoiseGate \
    qamplifier.h \
    QAmplifier \
    qsignalgenerator.h \
    QSignalGenerator \
    qequalizercontrol.h \
    QEqualizerControl
