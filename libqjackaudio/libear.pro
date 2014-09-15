TEMPLATE = lib
CONFIG += staticlib flat
TARGET = ear
DESTDIR = lib
OBJECTS_DIR = obj
MOC_DIR = moc
INCLUDEPATH += include include/jnoise
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3

win32 {
    INCLUDEPATH += ../3rdparty/fftw/include \
                   ../3rdparty/jack/include
}

SOURCES += \
    src/earprocessor.cpp \
    src/fftwadapter.cpp \
    src/jackadapter.cpp \
    src/digitalequalizer.cpp \
    src/jnoise/jnoise.cpp \
    src/jnoise/randomgenerator.cpp
HEADERS += \
    include/processor.h \
    include/earprocessor.h \
    include/fftwadapter.h \
    include/jackadapter.h \
    include/digitalequalizer.h \
    include/jnoise/prbsgenerator.h \
    include/jnoise/randomgenerator.h \
    include/jnoise/jnoise.h
