TEMPLATE = lib
CONFIG += staticlib flat
TARGET = qjackaudio
DESTDIR = lib
OBJECTS_DIR = obj
MOC_DIR = moc
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3

SOURCES += \
    system.cpp \
    buffer.cpp \
    client.cpp \
    driver.cpp \
    parameter.cpp \
    port.cpp \
    server.cpp

HEADERS += \
    system.h \
    System \
    Buffer \
    buffer.h \
    Client \
    client.h \
    Driver \
    driver.h \
    Parameter \
    parameter.h \
    Port \
    port.h \
    Server \
    server.h \
    processor.h \
    Processor
