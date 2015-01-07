TEMPLATE = lib
CONFIG += staticlib flat
TARGET = qjackaudio
DESTDIR = lib
OBJECTS_DIR = obj
MOC_DIR = moc
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3

SOURCES += \
    qjackclient.cpp \
    qjackport.cpp \
    qjackdriver.cpp \
    qjackparameter.cpp \
    qjackserver.cpp \
    qjack.cpp \
    dsp/qamplifier.cpp \
    dsp/qcompressor.cpp \
    dsp/qequalizer.cpp \
    dsp/qequalizercontrol.cpp \
    dsp/qfftw.cpp \
    dsp/qnoisegate.cpp \
    dsp/qsignalgenerator.cpp \
    dsp/qunits.cpp \
    qjackbuffer.cpp

HEADERS += \
    QAudioProcessor \
    qaudioprocessor.h \
    QJackClient \
    qjackclient.h \
    QJackPort \
    qjackport.h \
    qjackdriver.h \
    QJackDriver \
    qjackparameter.h \
    QJackParameter \
    QJackServer \
    qjackserver.h \
    qjack.h \
    QJack \
    dsp/QAmplifier \
    dsp/qamplifier.h \
    dsp/QCompressor \
    dsp/qcompressor.h \
    dsp/QDigitalFilter \
    dsp/qdigitalfilter.h \
    dsp/QEqualizer \
    dsp/qequalizer.h \
    dsp/QEqualizerControl \
    dsp/qequalizercontrol.h \
    dsp/QFFTW \
    dsp/qfftw.h \
    dsp/QNoiseGate \
    dsp/QSignalGenerator \
    dsp/qsignalgenerator.h \
    dsp/QUnits \
    dsp/qunits.h \
    dsp/qnoisegate.h \
    QJackBuffer \
    qjackbuffer.h
